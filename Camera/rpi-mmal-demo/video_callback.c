#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
// #include <math.h>
#include <stdint.h>

#include "bcm_host.h"
#include "interface/vcos/vcos.h"

#include "interface/mmal/mmal.h"
#include "interface/mmal/util/mmal_default_components.h"
#include "interface/mmal/util/mmal_connection.h"

#include "video_callback.h"
#include "is_options.h"
#include "yuv.h"

#define TEMP_IMAGE

unsigned char img2_bitplanes[1280*720*3/2];

// Preview callback function goes here
void video_buffer_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
  struct timespec t1, t2;
  static struct timespec tpf;
  struct timespec tcf;
  static long t_sum_ms = 0;
  static int loop = 0;
  static YUV_IMAGE_T img, img2;
  int translate_image = 0;
  
  clock_gettime(CLOCK_MONOTONIC, &tcf);
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);

  int w=1280, h=720;

  MMAL_BUFFER_HEADER_T *new_buffer;
  MMAL_BUFFER_HEADER_T *preview_new_buffer;
  MMAL_POOL_T *pool = (MMAL_POOL_T *) port->userdata;

  // Target color 
  static YUV_T target = {0, 135, 64};   // Green paper
  //  {0, 101, 192};      // Orange Cap
  //  {0, 90, 158};       // Blue Cap

  loop++;

  preview_new_buffer = mmal_queue_get(preview_input_port_pool->queue);
  if (preview_new_buffer) {
    
    int Y_array_size = w*h;
    int UV_array_size = Y_array_size/4;
    int x, y; // coordinates

    int i;
    unsigned char * Y = preview_new_buffer->data;
    unsigned char * U = &(preview_new_buffer->data[Y_array_size]);
    unsigned char * V = &(preview_new_buffer->data[Y_array_size + UV_array_size]);


    // copy all: Y, U, V
    memcpy(preview_new_buffer->data, buffer->data, Y_array_size + 2*UV_array_size);
    
    // Y: luminance.
    // Invert Luminance
    //    unsigned int * Y32 = (unsigned int * ) Y;
    //    do {
    //      *Y32 ^= 0xffffffff;
    //      Y++;
    //    }  while (Y < U);
    
    YUV_Image_Init(&img, (unsigned char *) (preview_new_buffer->data), w, h);
    YUV_Image_Init(&img2, img2_bitplanes, w, h);
    

    if (is_invert) {
      // Invert brightness of central rectangle
      for (y = h/2 - H_INVERT/2; y < h/2 + H_INVERT/2; y++) {
	for (x = w/2 - W_INVERT/2; x < w/2 + W_INVERT/2; x++) {
	  Y[y*w + x] ^= 0xff;
	}
      }
    }

    int sep =20; 
    int wc = w/2, hc = h/2;

    // Clear screen
#ifdef NUM_DISPLAY
    printf("\033[2J");
#endif
    YUV_T center_color;
    Draw_Circle(&img, wc, hc, 10, &black, 0);    Draw_Circle(&img, wc, hc, 12, &orange, 0);
    Draw_Circle(&img, wc, hc, 13, &orange, 0);    Draw_Circle(&img, wc, hc, 22, &black, 0);
    Draw_Circle(&img, wc, hc, 24, &white, 0);    Draw_Circle(&img, wc, hc, 25, &white, 0);
		
    Get_Pixel_yuv(&img, wc, hc, &center_color);
    if (show_data > 0)
      printf("\nCenter pixel: (%d, %d, %d)\n", center_color.y, center_color.u, center_color.v); 

    if (update_target_color) {
      update_target_color = 0;
      Get_Pixel_yuv(&img, wc, hc, &target);
      printf("\nUpdated target color: (%d, %d, %d)\n",
	     target.y, target.u, target.v); 
    }
          
    // Highlight color
    int u_hl = 200;
    int v_hl = 200;
    int centroid_x=0, centroid_y=0, num_matches=0;
    int offsetX=0, offsetY=0;
    
    for (y = sep/2; y <= h-sep/2; y += sep) { 
      for (x = sep/2; x <= w-sep/2; x += sep) {
	YUV_T color;
	Get_Pixel_yuv(&img, x,y, & color);
	// Identify pixels with right color
	int diff = Sq_UV_Difference_yuv(&color, &target);
	if (diff < color_threshold) {
	  centroid_x += x;
	  centroid_y += y;
	  num_matches++;
	  Draw_Rectangle(&img, x, y, sep-2, sep-2, &pink, 0);
	}
      }
    }
    // Show centroid
    if (num_matches > 0) {
      centroid_x /= num_matches;
      centroid_y /= num_matches;
      if (show_data > 0) {
	printf("Match centroid at (%d, %d) for %d samples\n",
	       centroid_x, centroid_y, num_matches);
      }
      Draw_Circle(&img, centroid_x, centroid_y, 10, &white, 1);
      offsetX = centroid_x - wc;
      offsetY = centroid_y - hc;
      if (show_data > 0) {
	printf("Offset = %d, %d\n", offsetX, offsetY);
      }
      translate_image = 1;
    } else {
      translate_image = 0;
    }

    if (translate_image) {
      // Copy bitplanes of image so far into another buffer
      YUV_Image_Copy(&img2, &img);
      target.y = 128;
      YUV_Image_Fill(&img, &target);
      YUV_Translate_Image(&img, &img2, -offsetX, -offsetY, 0);
    }
    
    preview_new_buffer->length = buffer->length;
    // Send modified image in new buffer to preview component
    if (mmal_port_send_buffer(preview_input_port, preview_new_buffer) != MMAL_SUCCESS) {
      printf("ERROR: Unable to send buffer \n");
    }
  } else {
    printf("ERROR: mmal_queue_get (%d)\n", preview_new_buffer);
  }
      
  mmal_buffer_header_release(buffer);

  // and send one back to the port (if still open)
  if (port->is_enabled) {
    MMAL_STATUS_T status;

    new_buffer = mmal_queue_get(pool->queue);
    if (new_buffer)
      status = mmal_port_send_buffer(port, new_buffer);
    if (!new_buffer || status != MMAL_SUCCESS)
      printf("Unable to return a buffer to the video port\n");
  }

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2);
  long t = t2.tv_nsec - t1.tv_nsec;
  if (t<0)
    t += 1000000000;
  if (loop > 1) {
    long period = (tcf.tv_nsec - tpf.tv_nsec);
    if (period < 0)
      period += 1000000000;
    if (show_data > 2)
      printf("Frame processing time: %.3f of %.3f ms\n", t/1000000.0, period/1000000.0);
    //    fprintf(fp, "%9d\n", t); // ns 
  }
  t_sum_ms += t/1000000;

  if (show_data > 0) {
    if ((loop & 0x0f) == 0) {
      //    fflush(fp);
      printf("Average frame processing time %.3f ms\n", ((float) t_sum_ms)/loop);
    }
  }
  tpf = tcf;
}
