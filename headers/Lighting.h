#include <SDL2/SDL.h>

#include "Scene.h"
#include "STL_Handler.h"
#include "Linear_Algebra_Utils.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef C6912C25_B029_4CD0_89E5_A784ACF95509
#define C6912C25_B029_4CD0_89E5_A784ACF95509

typedef struct {
    Vertex3D* location;
    Uint32 color;
    double intensity;
} LightSource;

LightSource* build_light_source(Vertex3D* location, Uint32 color, double intensity);
void destroy_light_source(LightSource* light);

#endif /* C6912C25_B029_4CD0_89E5_A784ACF95509 */
