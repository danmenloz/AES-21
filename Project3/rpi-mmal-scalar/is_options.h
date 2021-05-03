#ifndef IS_OPTIONS_H
#define IS_OPTIONS_H
#include <stdint.h>

#define H_INVERT_RECT (400)
#define W_INVERT_RECT (800)

extern int run;
extern int invert, invert_rect;
extern int update_target_color;
extern int show_data;
extern int highlight_matches;
extern int color_threshold;
extern int imstab_servo;
extern int imstab_digital;
extern const int chroma_subsample_sep;

extern uint8_t ServoTiltDegree;
extern uint8_t ServoPanDegree;

#endif // IS_OPTIONS_H
