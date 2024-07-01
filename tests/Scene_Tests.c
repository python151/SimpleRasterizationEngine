#include <stdio.h>

#include "../source/Scene.c"
#include "../source/STL_Handler.c"

int main(int argc, char* argv[]) {
    puts("Creating camera...");
    Vertex3D* p = build_vertex3D_struct(1, 2, 3);
    Camera* camera = build_camera_struct(p, 4, 5, 6, 60);
    printf("\tcamera->fov = %.2f\n", camera->fov);
    printf("\tcamera->point->x = %.2f\n", camera->point->x);
    printf("\tcamera->rotation[0] = %.2f\n", camera->rotation[0]);
    printf("\tcamera->rotation[1] = %.2f\n", camera->rotation[1]);
    printf("\tcamera->rotation[2] = %.2f\n", camera->rotation[2]);
    
    puts("Creating model..."); 
    FILE* file = fopen("../assets/20mm_cube.stl", "r");
    Model* m = build_model_struct(file);
    fclose(file);

    puts("Constructing scene...");
    Scene* scene = build_scene_struct(camera, &m, 1);
    printf("\tscene->camera->fov = %.2f\n", scene->camera->fov);
    printf("\tscene->models[0]->triangle_count = %ld\n", scene->models[0]->triangle_count);
    printf("\tscene->num_models = %ld\n", scene->num_models);


    puts("Destroying scene...");
    destroy_scene_recursively(scene);

    puts("Creating image...");
    Image2D* image = build_empty_image_struct(1, 1, 10, 10);

    puts("Destroying image...");
    destoy_image_struct(image);

    return 0;
}
