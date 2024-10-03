#include "headers/Lighting.h"

LightSource* build_light_source(Vertex3D* location, Uint32 color, double intensity) {
    LightSource* light = malloc(sizeof(LightSource));
    if (light == NULL)
        return NULL;
    
    light->color = color;
    light->intensity = intensity;
    light->location = location;

    return light;
}
void destroy_light_source(LightSource* light) {
    // TODO: IMPLEMENT THIS
}