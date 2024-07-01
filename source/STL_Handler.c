#include "STL_Handler.h"

#include <stdlib.h>
#include <stdbool.h>

Triangle* build_triangle_struct_no_norm(int a, int b, int c) {
    Triangle* triangle = malloc(sizeof(Triangle));

    triangle->indices[0] = a;
    triangle->indices[1] = b;
    triangle->indices[2] = c;

    triangle->normal[0] = 0;
    triangle->normal[1] = 0;
    triangle->normal[2] = 0;

    return triangle;
}
Triangle* build_triangle_struct(int a, int b, int c, double n1, double n2, double n3) {
    Triangle* triangle = malloc(sizeof(Triangle));

    triangle->indices[0] = a;
    triangle->indices[1] = b;
    triangle->indices[2] = c;

    triangle->normal[0] = n1;
    triangle->normal[1] = n2;
    triangle->normal[2] = n3;

    return triangle;
}
void destroy_triangle_struct(Triangle* object) {
    free(object);
}

Vertex3D* build_vertex3D_struct(double x, double y, double z) {
    Vertex3D* vertex = malloc(sizeof(Vertex3D));
    vertex->x = x;
    vertex->y = y;
    vertex->z = z;
    return vertex;
}
void destroy_vertex3D_struct(Vertex3D* object) {
    free(object);
}

Vertex2D* build_vertex2D_struct(double x, double y) {
    Vertex2D* vertex = malloc(sizeof(Vertex2D));
    vertex->x = x;
    vertex->y = y;
    return vertex;
}
void destroy_vertex2D_struct(Vertex2D* object) {
    free(object);
}

bool vertex_3d_isequal(Vertex3D v1, Vertex3D v2) {
    return (v1.x == v2.x) && (v1.y = v2.y) && (v1.z == v2.z);
}
bool vertex_3d_raw_isequal(Vertex3D v1, double x, double y, double z) {
    return (v1.x == x) && (v1.y == y) && (v1.z == z);
}

void build_triangle(FILE* file, char line_buffer[250], Model* current_model) {
    double n1, n2, n3;
    double x1, y1, z1;
    double x2, y2, z2;
    double x3, y3, z3;

    int line1_matched = sscanf(line_buffer, "facet normal %lf %lf %lf ", &n1, &n2, &n3);
    if (line1_matched != 3)
        puts("Warning: STL file triangle line 1 potential mismatch");
    
    fgets(line_buffer, 250*sizeof(char), file); // Skip this line

    int line3_matched = fscanf(file, "vertex %lf %lf %lf ", &x1, &y1, &z1);
    if (line3_matched != 3)
        puts("Warning: STL file triangle line 3 potential mismatch");

    int line4_matched = fscanf(file, "vertex %lf %lf %lf ", &x2, &y2, &z2);
    if (line4_matched != 3)
        puts("Warning: STL file triangle line 4 potential mismatch");
    
    int line5_matched = fscanf(file, "vertex %lf %lf %lf ", &x3, &y3, &z3);
    if (line5_matched != 3)
        puts("Warning: STL file triangle line 5 potential mismatch");
    
    fgets(line_buffer, 250*sizeof(char), file); // Skip this line
    fgets(line_buffer, 250*sizeof(char), file); // Skip this line

    // Find indices, if none exists, then create new vertices for them
    int indices[3] = {-1};
    for (int i = 0; i < current_model->vertex_count; i++) {
        Vertex3D v = *current_model->vertices[i];
        if (vertex_3d_raw_isequal(v, x1, y1, z1))
            indices[0] = i;
        if (vertex_3d_raw_isequal(v, x2, y2, z2))
            indices[1] = i;
        if (vertex_3d_raw_isequal(v, x3, y3, z3))
            indices[2] = i;
    }
    if (indices[0] == -1) {
        Vertex3D* v = build_vertex3D_struct(x1, y1, z1);
        current_model->vertex_count++;
        current_model->vertices = realloc(current_model->vertices, current_model->vertex_count*sizeof(Vertex3D*));
        current_model->vertices[current_model->vertex_count-1] = v;
        indices[0] = current_model->vertex_count-1;
    }
    if (indices[1] == -1) {
        Vertex3D* v = build_vertex3D_struct(x2, y2, z2);
        current_model->vertex_count++;
        current_model->vertices = realloc(current_model->vertices, current_model->vertex_count*sizeof(Vertex3D*));
        current_model->vertices[current_model->vertex_count-1] = v;
        indices[1] = current_model->vertex_count-1;
    }
    if (indices[2] == -1) {
        Vertex3D* v = build_vertex3D_struct(x3, y3, z3);
        current_model->vertex_count++;
        current_model->vertices = realloc(current_model->vertices, current_model->vertex_count*sizeof(Vertex3D*));
        current_model->vertices[current_model->vertex_count-1] = v;
        indices[2] = current_model->vertex_count-1;
    }
    Triangle* t = build_triangle_struct(indices[0], indices[1], indices[2], n1, n2, n3);
    current_model->triangle_count++;
    current_model->triangles = realloc(current_model->triangles, current_model->triangle_count*sizeof(Triangle*));
    current_model->triangles[current_model->triangle_count-1] = t;
}

// Returns a single model, if multiple ascii models are passed, only the first one will be returned
Model* build_model_struct(FILE* file) {
    Model* current_model = NULL;

    char line_buffer[250];
    while(fgets(line_buffer, sizeof(line_buffer), file)) {
        if (strstr(line_buffer, "solid")) {
            if (current_model != NULL) 
                return current_model;
            current_model = malloc(sizeof(Model));
            current_model->triangle_count = 0;
            current_model->vertex_count = 0;
            current_model->triangles = NULL;
            current_model->vertices = NULL;
        }

        if (strstr(line_buffer, "facet normal")) {
            build_triangle(file, line_buffer, current_model);
        }
    }

    return current_model;
}
void destroy_model_struct(Model* object) {
    for (int t = 0; t < object->triangle_count; t++) {
        destroy_triangle_struct(object->triangles[t]);
    }
    for (int v = 0; v < object->vertex_count; v++) {
        destroy_vertex3D_struct(object->vertices[v]);
    }
    free(object->vertices);
    free(object->triangles);
    free(object);
}