#include <SDL2/SDL.h>
#include <stdio.h>

#include "../source/STL_Handler.c"

void PrintTriangleStruct(Triangle* t) {
    puts("Printing triangle struct...");
    printf("\ttriangle->indices[0] = %d\n", t->indices[0]);
    printf("\ttriangle->indices[1] = %d\n", t->indices[1]);
    printf("\ttriangle->indices[2] = %d\n", t->indices[2]);
    printf("\ttriangle->normal[0] = %.2f\n", t->normal[0]);
    printf("\ttriangle->normal[1] = %.2f\n", t->normal[1]);
    printf("\ttriangle->normal[2] = %.2f\n", t->normal[2]);
    puts("Done printing triangle struct");
}

void PrintVertex2DStruct(Vertex2D* v) {
    puts("Printing Vertex2D struct...");
    printf("\tv->x = %.2f\n", v->x);
    printf("\tv->y = %.2f\n", v->y);
    puts("Done printing Vertex2D struct");
}

void PrintVertex3DStruct(Vertex3D* v) {
    puts("Printing Vertex3D struct...");
    printf("\tv->x = %.2f\n", v->x);
    printf("\tv->y = %.2f\n", v->y);
    printf("\tv->z = %.2f\n", v->z);
    puts("Done printing Vertex3D struct");
}

int main(int argc, char* argv[]) {
    Triangle* t1 = build_triangle_struct_no_norm(1, 2, 3);
    PrintTriangleStruct(t1);
    destroy_triangle_struct(t1);

    Triangle* t2 = build_triangle_struct(2, 3, 4, 1.1, 1.2, 4.3);
    PrintTriangleStruct(t2);
    destroy_triangle_struct(t2);

    Vertex3D* v1 = build_vertex3D_struct(1.0, 2.0, 3.0);
    PrintVertex3DStruct(v1);
    destroy_vertex3D_struct(v1);

    Vertex2D* v2 = build_vertex2D_struct(1.0, 2.0);
    PrintVertex2DStruct(v2);
    destroy_vertex2D_struct(v2);

    puts("Opening file...");
    FILE* file = fopen("../assets/20mm_cube.stl", "r");
    puts("Building model...");
    Model* m = build_model_struct(file);
    fclose(file);
    printf("Our total vertex count is %ld\n", m->vertex_count);
    printf("Our triangle count is %ld\n", m->triangle_count);
    
    puts("Destroying model...");
    destroy_model_struct(m);

    return 0;
}
