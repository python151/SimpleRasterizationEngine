#include "Projections.h"
#include "Transformation_Utils.h"
#include "stdlib.h"
#include <math.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Matrix* generate_perspective_projection_matrix(double fov, int w, int h) {
    double radians = fov * (M_PI / 180.0);
    double d = w / 2*tan(fov/2);
    
    Matrix* mat = construct_translation_matrix(0, 0, 0); // This builds a 4x4 identity matrix
    mat->values[0] = d;
    mat->values[5] = d;
    mat->values[2] = w/2;
    mat->values[6] = h/2;
    return mat;
}

Image2D* construct_image_buffer(Scene* scene, int w, int h) {
    size_t num_vertices = 0;
    size_t num_trianges = 0;

    for (int i = 0; i < scene->num_models; i++) {
        num_vertices += scene->objects[i]->model->vertex_count;
        num_trianges += scene->objects[i]->model->triangle_count;
    }

    Image2D* image_buffer = build_empty_image_struct(num_trianges, num_vertices, h, w);

    int curr_tri = 0;
    for (int i = 0; i < scene->num_models; i++) {
        for (int j = 0; j < scene->objects[i]->model->triangle_count; j++) {
            image_buffer->triangles[curr_tri] = scene->objects[i]->model->triangles[j];
            curr_tri++;
        }
    }

    for (int i = 0; i < num_vertices; i++) {
        image_buffer->vertices[i] = build_vertex2D_struct(0, 0);
    }
    
    return image_buffer;
}