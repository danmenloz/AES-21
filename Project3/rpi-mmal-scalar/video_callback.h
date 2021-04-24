#ifndef VIDEO_CALLBACK_H
#define VIDEO_CALLBACK_H

#define W_INVERT 1280
#define H_INVERT 720

#define ASCII_ESC 27
// #define NUM_DISPLAY 

extern FILE * fp;
extern MMAL_POOL_T *preview_input_port_pool;
extern MMAL_PORT_T *preview_input_port;

void video_buffer_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);

#endif
