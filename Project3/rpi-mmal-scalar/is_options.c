#include "is_options.h"

int run=1;
int invert=0, invert_rect=0;
int update_target_color=0;
int show_data=1;
int color_threshold=300;
uint8_t ServoTiltDegree = 100;
uint8_t ServoPanDegree = 0;
int imstab_servo = 0;
int imstab_digital = 0;
const int chroma_subsample_sep = 2; // going below 2 has no effect, since chroma is 1/2 subsampled already
