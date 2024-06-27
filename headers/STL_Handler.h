#include <stddef.h>
#include <stdio.h>

#ifndef E832129C_657D_4EED_9503_7E694ADC658B
#define E832129C_657D_4EED_9503_7E694ADC658B

typedef struct {
    double x, y, z;  // Coordinates of a vertex
} Vertex;

typedef struct {
    int indices[3];  // Indices of the three vertices that form the triangle
} Triangle;

typedef struct {
    Vertex* vertices;  // Array of vertices
    size_t vertex_count;  // Number of vertices
    Triangle* triangles;  // Array of triangles
    size_t triangle_count;  // Number of triangles
} STL_Model;

Triangle* build_triangle_struct(int a, int b, int c);
void destroy_triangle_struct(Triangle* object);

Vertex* build_vertex_struct(double x, double y, double z);
void destroy_vertex_struct(Vertex* object);

STL_Model* build_model_struct(const FILE* file);
void destroy_model_struct(STL_Model* object);


#endif /* E832129C_657D_4EED_9503_7E694ADC658B */
