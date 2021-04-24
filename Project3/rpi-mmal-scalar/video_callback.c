#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
// #include <math.h>
#include <stdint.h>
#include <sys/param.h>

#include "bcm_host.h"
#include "interface/vcos/vcos.h"

#include "interface/mmal/mmal.h"
#include "interface/mmal/util/mmal_default_components.h"
#include "interface/mmal/util/mmal_connection.h"

#include "video_callback.h"
#include "is_options.h"
#include "yuv.h"

#include "PCA9685_servo_driver.h"

unsigned char img2_bitplanes[1280*720*3/2];

void draw_overlay_info(YUV_IMAGE_T * i) {
  Draw_Circle(i, i->half_w, i->half_h, 10, &black, 0);
  Draw_Circle(i, i->half_w, i->half_h, 12, &orange, 0);
  Draw_Circle(i, i->half_w, i->half_h, 13, &orange, 0);
  Draw_Circle(i, i->half_w, i->half_h, 22, &black, 0);
  Draw_Circle(i, i->half_w, i->half_h, 24, &white, 0);
  Draw_Circle(i, i->half_w, i->half_h, 25, &white, 0);
}

void clear_term_screen(void) {
  printf("\033[2J");
}

int find_chroma_matches(YUV_IMAGE_T * i, YUV_T * tc, int * rcx, int * rcy, int sep){
  int x, y;
  int matches=0;
  unsigned int min_x=0xffffffff, max_x=0, min_y=0xffffffff, max_y=0;
  int offsetX=0, offsetY=0;
  YUV_T color;
  int cx=0, cy=0;
  
  for (y = sep/2; y <= i->h - sep/2; y += sep) { 
    for (x = sep/2; x <= i->w - sep/2; x += sep) {
      Get_Pixel_yuv(i, x,y, & color);
      // Identify pixels with right color
      int diff = Sq_UV_Difference_yuv(&color, tc);
      if (diff < color_threshold) {
	cx += x;
	cy += y;
	min_x = MIN(min_x, x);
	max_x = MAX(max_x, x);
	min_y = MIN(min_y, y);
	max_y = MAX(max_y, y);

	matches++;
	if (sep > 10)
	  Draw_Rectangle(i, x, y, sep-2, sep-2, &pink, 0);
	else {
	  Draw_Line(i, x-sep/2, y, x+sep/2, y, &pink);
	  Draw_Line(i, x, y-sep/2, x, y+sep/2, &pink);
	}
      }
    }
  }
  if (matches > 0) {
#if 1
    cx /= matches;
    cy /= matches;
#else
    cx = (max_x+min_x)/2;
    cy = (max_y+min_y)/2;
#endif
  }
  *rcx = cx;
  *rcy = cy;
      
  return matches;
}

void video_buffer_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
  struct timespec t1, t2;
  static struct timespec tpf;
  struct timespec tcf;
  static long t_sum_ms = 0;
  static int loop = 0;
  static YUV_IMAGE_T img, img2;
  int translate_image = 0;
  int w=1280, h=720;
  // Default target color 
  static YUV_T target = {128, 135, 64};   // Green paper
  
  clock_gettime(CLOCK_MONOTONIC, &tcf);
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);

  MMAL_BUFFER_HEADER_T *new_buffer;
  MMAL_BUFFER_HEADER_T *preview_new_buffer;
  MMAL_POOL_T *pool = (MMAL_POOL_T *) port->userdata;

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

    // copy image components to preview_new_buffer
    memcpy(preview_new_buffer->data, buffer->data, Y_array_size + 2*UV_array_size);
    preview_new_buffer->length = buffer->length;
  
    // initialize YUV_IMAGE_T data structures describing images
    YUV_Image_Init(&img, (unsigned char *) (preview_new_buffer->data), w, h); // original image
    YUV_Image_Init(&img2, img2_bitplanes, w, h); // extra space for modified image
    
    if (invert) { // Y: luminance.
      // Invert Luminance, one word at a time
      unsigned int * Y32 = (unsigned int * ) Y;
      do {
	*Y32 ^= 0xffffffff;
	Y32++;
      }  while (Y32 < (unsigned int *) U); // Note: U must be a multiple of 4
    } else if (invert_rect) {
      // Invert brightness of central rectangle, one byte at a time
      for (y = h/2 - H_INVERT_RECT/2; y < h/2 + H_INVERT_RECT/2; y++) {
	for (x = w/2 - W_INVERT_RECT/2; x < w/2 + W_INVERT_RECT/2; x++) {
	  Y[y*w + x] ^= 0xff;
	}
      }
    }

    YUV_T center_color;
    Get_Pixel_yuv(&img, img.half_w, img.half_h, &center_color);
    if (show_data > 0)
      printf("\nCenter pixel: (%d, %d, %d)\n", center_color.y, center_color.u, center_color.v); 
    if (update_target_color) {
      target = center_color;
      update_target_color = 0;
      printf("\nUpdated target color: (%d, %d, %d)\n", target.y, target.u, target.v); 
    }

    // draw center circles
    draw_overlay_info(&img);
    
    // Find area matching target color
    int centroid_x, centroid_y, num_matches, offsetX, offsetY;
    num_matches = find_chroma_matches(&img, &target, &centroid_x, &centroid_y, chroma_subsample_sep);
    if (num_matches > 0) {  
      // Show centroid
      Draw_Circle(&img, centroid_x, centroid_y, 10, &white, 1);
      offsetX = img.half_w - centroid_x;
      offsetY = img.half_h - centroid_y;
      if (show_data > 0) {
	printf("Match centroid at (%d, %d) for %d samples\n",
	       centroid_x, centroid_y, num_matches);
	printf("Offset = %d, %d\n", offsetX, offsetY);
      }
      // Correct image position
      if (imstab_digital) {
	// Copy bitplanes of image so far into another buffer
	YUV_Image_Copy(&img2, &img);
	//	target.y = 128; // overwrite 
	YUV_Image_Fill(&img, &target);
	YUV_Translate_Image(&img, &img2, offsetX, offsetY, 0);
      }
      if (imstab_servo) {
	Update_Servo(offsetX, offsetY);
      }
    }
    
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
  }
  t_sum_ms += t/1000000;

  if (show_data > 0) {
    if ((loop & 0x0f) == 0) {
      printf("Average frame processing time %.3f ms\n", ((float) t_sum_ms)/loop);
    }
  }
  tpf = tcf;
}
