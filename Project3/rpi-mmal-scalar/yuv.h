#ifndef YUV_H
#define YUV_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

typedef struct {
  uint8_t y;
  uint8_t u;
  uint8_t v;
} YUV_T;

extern YUV_T black, white, green, orange, blue, pink;

typedef struct {
  int w, h, half_w, half_h;
  uint8_t *bY;
  uint8_t *bU;
  uint8_t *bV;
} YUV_IMAGE_T;


void YUV_Image_Init(YUV_IMAGE_T * i, unsigned char * buffer, int width, int height);
void YUV_Image_Info(YUV_IMAGE_T * i);
void Set_Pixel(YUV_IMAGE_T * i, int px, int py, uint8_t lum, uint8_t u, uint8_t v);
void Set_Pixel_yuv(YUV_IMAGE_T * i, int px, int py, YUV_T * yuv);
void Get_Pixel_yuv(YUV_IMAGE_T * i, int px, int py, YUV_T * yuv);


inline __attribute__((always_inline)) int Sq_UV_Difference_yuv(YUV_T * c1, YUV_T * c2){
  int du, dv;
  
  du = (int) c1->u - c2->u;
  dv = (int) c1->v - c2->v;
  return du*du + dv*dv;
}
void Draw_Line(YUV_IMAGE_T * i, int p1X, int p1Y, int p2X, int p2Y, YUV_T * color);
void Draw_Circle(YUV_IMAGE_T * i, int pcX, int pcY, int radius, YUV_T * c, int filled);
void Draw_Rectangle(YUV_IMAGE_T * i, int pcX, int pcY, int dX, int dY, YUV_T * c, int filled);

void YUV_Image_Copy(YUV_IMAGE_T * dst, YUV_IMAGE_T * src);
void YUV_Image_Fill(YUV_IMAGE_T * dst, YUV_T * c);

void YUV_Translate_Image(YUV_IMAGE_T * dst, YUV_IMAGE_T * src, int dX, int dY, float dTheta);
void Translate_Image(int dX, int dY, float dTheta);
#endif // YUV_H

