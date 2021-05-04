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
#include <arm_neon.h>
#include <arm_acle.h>

#include "PCA9685_servo_driver.h"

#define MIN_MAX_CENTROID 0

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
  short int x, y;
  int matches=0;
  #if MIN_MAX_CENTROID
    unsigned int min_x=0xffffffff, max_x=0, min_y=0xffffffff, max_y=0;
  #endif
  int offsetX=0, offsetY=0;
  YUV_T color;
  int cx=0, cy=0;
  int h = i->h, w = i->w, half_w = i->half_w;
  YUV_T marker = pink; // higlight color

  int y_end = h - sep/2;

  // Neon types
  int16x8_t V_target, V_image, U_target, U_image;
  uint16x8_t sum_pos_X, sum_pos_Y, pos_X_inc, sum_matches;
  uint8x8_t V_target_8, V_image_8, U_target_8, U_image_8, pos_X_inc_8; 
  uint8x8x2_t Y_image_l, Y_image_u; // This a 2 vector variable. Each vector has eight lanes of 8-bit data.
  uint16x4_t sum_u, sum_l;
  uint32x2_t sum;

  sum_pos_X = vdupq_n_u16(0);
  sum_pos_Y = vdupq_n_u16(0);
  sum_matches = vdupq_n_u16(0);

  pos_X_inc_8 = vcreate_u8(0x0E0C0A0806040200); //vcreate_u8(0x00020406080A0C0E);
  pos_X_inc = vmovl_u8(pos_X_inc_8);

  U_target_8 = vld1_dup_u8(&(tc->u));
  V_target_8 = vld1_dup_u8(&(tc->v));

  U_target = vreinterpretq_s16_u16(vmovl_u8(U_target_8));
  V_target = vreinterpretq_s16_u16(vmovl_u8(V_target_8));

  // for (y = sep/2; y <= 96; y += sep) { 
  //   for (x = 0; x < 96; x += 16) {
  for (y = sep/2; y <= y_end; y += sep) { 
    for (x = 0; x < w; x += 16) {
      int16_t vec[8];
      uint8_t vec8[8];
      
      Y_image_u = vld2_u8(&(i->bY[(y-1)*w + x]));
      Y_image_l = vld2_u8(&(i->bY[y*w + x]));
      // Y_image_l.val[0]
      // Y_image_l.val[1]

      U_image_8 = vld1_u8(&(i->bU[y/2*half_w + x/2]));
      V_image_8 = vld1_u8(&(i->bV[y/2*half_w + x/2]));

      /* Print vector */
      // vst1_u8(vec8, U_image_8);
      // printf("U8 : %d %d %d %d %d %d %d %d \n", vec8[0],vec8[1],vec8[2],vec8[3],vec8[4],vec8[5],vec8[6],vec8[7]);
      // vst1_u8(vec8, V_image_8);
      // printf("V8 : %d %d %d %d %d %d %d %d \n", vec8[0],vec8[1],vec8[2],vec8[3],vec8[4],vec8[5],vec8[6],vec8[7]);

      U_image = vreinterpretq_s16_u16(vmovl_u8(U_image_8));
      V_image = vreinterpretq_s16_u16(vmovl_u8(V_image_8));

      /* Print vector */
      // vst1q_s16(vec, U_image);
      // printf("U16: %d %d %d %d %d %d %d %d \n", vec[0],vec[1],vec[2],vec[3],vec[4],vec[5],vec[6],vec[7]);
      // vst1q_s16(vec, V_image);
      // printf("V16: %d %d %d %d %d %d %d %d \n", vec[0],vec[1],vec[2],vec[3],vec[4],vec[5],vec[6],vec[7]);

      int16x8_t du, dv, mu, mv, sq_uv_diff;
      uint16x8_t posX, posY, masked_pos_X, masked_pos_Y, mask;
      uint8x8_t mask_vis;

      du = vsubq_s16(U_image, U_target);
      dv = vsubq_s16(V_image, V_target);

      mu = vmulq_s16(du,du);
      mv = vmulq_s16(dv,dv);

      sq_uv_diff = vaddq_s16(mu, mv);
      
      mask = vcleq_s16(sq_uv_diff, vld1q_dup_s16(&(color_threshold)));


      /* Highlighting */
      Y_image_u.val[0] = vbsl_u8(vmovn_u16(mask), vld1_dup_u8(&(marker.y)), Y_image_u.val[0]);
      Y_image_u.val[1] = vbsl_u8(vmovn_u16(mask), vld1_dup_u8(&(marker.y)), Y_image_u.val[1]);
      Y_image_l.val[0] = vbsl_u8(vmovn_u16(mask), vld1_dup_u8(&(marker.y)), Y_image_l.val[0]);
      Y_image_l.val[1] = vbsl_u8(vmovn_u16(mask), vld1_dup_u8(&(marker.y)), Y_image_l.val[1]);

      U_image_8 = vbsl_u8(vmovn_u16(mask), vld1_dup_u8(&(marker.u)), U_image_8);
      V_image_8 = vbsl_u8(vmovn_u16(mask), vld1_dup_u8(&(marker.v)), V_image_8);

      vst2_u8(&(i->bY[(y-1)*w + x]), Y_image_u);
      vst2_u8(&(i->bY[y*w + x]), Y_image_l);

      vst1_u8(&(i->bU[y/2*half_w + x/2]), U_image_8);
      vst1_u8(&(i->bV[y/2*half_w + x/2]), V_image_8);


      mask = vandq_u16(mask, vdupq_n_u16(1));

      posX = vaddq_u16(pos_X_inc, vld1q_dup_u16(&x));
      posY = vld1q_dup_u16(&y);

      // /* Print vector */
      // vst1q_u16(vec, posX);
      // printf(" X: %d %d %d %d %d %d %d %d \n", vec[0],vec[1],vec[2],vec[3],vec[4],vec[5],vec[6],vec[7]);
      // vst1q_u16(vec, posY);
      // printf(" Y: %d %d %d %d %d %d %d %d \n", vec[0],vec[1],vec[2],vec[3],vec[4],vec[5],vec[6],vec[7]);
      // vst1q_u16(vec, mask);
      // printf(" m: %d %d %d %d %d %d %d %d \n", vec[0],vec[1],vec[2],vec[3],vec[4],vec[5],vec[6],vec[7]);

      masked_pos_X = vmulq_u16(mask, posX);
      masked_pos_Y = vmulq_u16(mask, posY);

      // /* Print vector */
      // vst1q_u16(vec, masked_pos_X);
      // printf("mX: %d %d %d %d %d %d %d %d \n", vec[0],vec[1],vec[2],vec[3],vec[4],vec[5],vec[6],vec[7]);
      // vst1q_u16(vec, masked_pos_Y);
      // printf("mY: %d %d %d %d %d %d %d %d \n\n", vec[0],vec[1],vec[2],vec[3],vec[4],vec[5],vec[6],vec[7]);

      sum_pos_X = vaddq_u16(sum_pos_X, masked_pos_X);
      sum_pos_Y = vaddq_u16(sum_pos_Y, masked_pos_Y);
      sum_matches = vaddq_u16(sum_matches, mask);

      // Sums have to happen inside the loop. O.W. lanes will overflow.
      // sum pos X
      sum_u = vget_high_u16(sum_pos_X);
      sum_l = vget_low_u16(sum_pos_X);

      sum_u = vpadd_u16(sum_u, sum_l);
      sum_u = vpadd_u16(sum_u, vdup_n_u16(0));

      sum = vpaddl_u16(sum_u);
      cx += vget_lane_u32(sum, 0);
      
      // sum pos Y
      sum_u = vget_high_u16(sum_pos_Y);
      sum_l = vget_low_u16(sum_pos_Y);

      sum_u = vpadd_u16(sum_u, sum_l);
      sum_u = vpadd_u16(sum_u, vdup_n_u16(0));

      sum = vpaddl_u16(sum_u);
      cy += vget_lane_u32(sum, 0);

      // Reset pos sums
      sum_pos_X = vdupq_n_u16(0);
      sum_pos_Y = vdupq_n_u16(0);

      // printf("x,y = %d,%d\n", x,y);

      /*
      Get_Pixel_yuv(i, x,y, & color);
      // Identify pixels with right color
      int diff = Sq_UV_Difference_yuv(&color, tc);
      if (diff < color_threshold) {
        cx += x;
        cy += y;
        #if MIN_MAX_CENTROID
        min_x = MIN(min_x, x);
        max_x = MAX(max_x, x);
        min_y = MIN(min_y, y);
        max_y = MAX(max_y, y);
        #endif

        matches++;
        if (highlight_matches){
          if (sep > 10)
            Draw_Rectangle(i, x, y, sep-2, sep-2, &pink, 0);
          else {
            Draw_Line(i, x-sep/2, y, x+sep/2, y, &pink);
            Draw_Line(i, x, y-sep/2, x, y+sep/2, &pink);
          }
        }
      }*/
    }
  }

  // sum matches
  // Safe to sum here since lanes didn't overflow
  sum_u = vget_high_u16(sum_matches);
  sum_l = vget_low_u16(sum_matches);

  sum_u = vpadd_u16(sum_u, sum_l);
  sum_u = vpadd_u16(sum_u, vdup_n_u16(0));

  sum = vpaddl_u16(sum_u);
  matches = vget_lane_u32(sum, 0);
  
  // printf("matches:%d   cx:%d   cy:%d\n", matches,cx,cy);

  if (matches > 0) {
#if MIN_MAX_CENTROID
    cx = (max_x+min_x)/2;
    cy = (max_y+min_y)/2;
#else
    cx /= matches;
    cy /= matches;
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
  static double t_sum_ms = 0;
  static int loop = 0;
  static YUV_IMAGE_T img, img2;
  int translate_image = 0;
  int w=1280, h=720;
  // Default target color 
  static YUV_T target = {64, 120, 197}; //red
  
  clock_gettime(CLOCK_MONOTONIC, &tcf);
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);

  MMAL_BUFFER_HEADER_T *new_buffer;
  MMAL_POOL_T *pool = (MMAL_POOL_T *) port->userdata;

  loop++;
  
  int Y_array_size = w*h;
  int UV_array_size = Y_array_size/4;
  int x, y; // coordinates

  int i;
  unsigned char * Y = buffer->data;
  unsigned char * U = &(buffer->data[Y_array_size]);
  unsigned char * V = &(buffer->data[Y_array_size + UV_array_size]);

  // initialize YUV_IMAGE_T data structures describing images
  YUV_Image_Init(&img, (unsigned char *) (buffer->data), w, h); // original image
  YUV_Image_Init(&img2, img2_bitplanes, w, h); // extra space for modified image
  
  Draw_Rectangle(&img, 1280/2+100, 720/2-100, 80, 80, &red, 1);
  // Draw_Rectangle(&img, 40, 40, 80, 80, &red, 1);

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
  if (show_data > 3)
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
    if (show_data > 2) {
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
  if (mmal_port_send_buffer(preview_input_port, buffer) != MMAL_SUCCESS) {
    printf("ERROR: Unable to send buffer \n");
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
    if (show_data > 3)
      printf("Frame processing time: %.3f of %.3f ms\n", t/1000000.0, period/1000000.0);
  }
  t_sum_ms += t/1000000;

  if (show_data > 0) {
    if ((loop & 0x0F) == 0) { // change display frequency here!
      printf("%4d Frame processing times:  Cur. %.6f ms  |  Avg. %.6f ms  |  matches: %d\n", loop, t/1000000.0, ((double) t_sum_ms)/loop, num_matches);
    }
  }
  tpf = tcf;
}
