#include "Scene.h"

#ifndef B2F3F715_1BB8_4D31_AD9A_7603AFE07350
#define B2F3F715_1BB8_4D31_AD9A_7603AFE07350

typedef struct {
} Fragment;

typedef struct {
    int R, G, B;
} Pixel;

void rasterize(Fragment* fragment_buffer, Image2D* image, int w, int h);
void apply_textures(Pixel* frame_buffer, Fragment* fragment_buffer, int w, int h); 

#endif /* B2F3F715_1BB8_4D31_AD9A_7603AFE07350 */
