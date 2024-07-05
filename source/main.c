#include <SDL2/SDL.h>

#include "STL_Handler.h"
#include "Scene.h"
#include "Rasterizer.h"
#include "Projections.h"
#include "Transformation_Utils.h"
#include "Linear_Algebra_Utils.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int w = 800, h = 600;

    /* Initializes SDL Stuff */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Rasterizer",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          w, h,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Sets up framebuffer and necessary SDL structures for pixel rendering
    Uint32* framebuffer;
    framebuffer = malloc(w*h*sizeof(framebuffer));
    if (framebuffer == NULL)
        puts("Framebuffer allocation failure");
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);


    // Load models into memory
    FILE* file = fopen("assets/20mm_cube.stl", "r");
    Model* m = build_model_struct(file);
    fclose(file);
    GameObject* object = build_gameobject(build_vertex3D_struct(0, 0, 0), m);
    object->rotation[0] = 0;//3.14/10;//3.14/6;
    object->rotation[1] = 0;//3.14/6;
    object->rotation[2] = 0;//.3;//3.14/6;

    // List of pointers to each GameObject, will get free'd when we call destroy_scene_recursively()
    GameObject** objects = malloc(sizeof(GameObject*));
    objects[0] = object;

    // Construct a scene from these
    puts("Building scene");
    Vertex3D* p = build_vertex3D_struct(10, 30, -130);
    Camera* camera = build_camera_struct(p, -.3, 0, 0, 60);
    Scene* scene = build_scene_struct(camera, objects, 1);
    puts("Building scene 1");
    Image2D* image_buffer = construct_image_buffer(scene, w, h);
    puts("Building scene 2");

    double camera_theta = 0;

    Uint32 lastTime = 0, currentTime;
    float deltaTime;
    
    /* Main loop */
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Calculate delta time
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;  // Convert milliseconds to seconds
        lastTime = currentTime;

        // Update scene
        // [IN DEVELOPMENT - NOT IMPLEMENTED YET]
        //object->rotation[1] += .05;
        //camera->point->x = 100*cos(camera_theta);
        //camera->point->z = 100*sin(camera_theta);
        //camera->rotation[1] = camera_theta;
        //camera_theta += 3.14/40 * deltaTime;
        //printf("cam: %.2f\n", camera_theta);

        Matrix* projection_matrix = generate_perspective_projection_matrix(60, w, h);
        size_t current_vertex = 0;
        for (int i = 0; i < scene->num_models; i++) {
            
            // Find total delta (translation, rotation, etc. to go from model space into world space and then with respect to the camera)
            double dx = -scene->camera->point->x;
            double dy = -scene->camera->point->y;
            double dz = -scene->camera->point->z;
            // Construct transformation matrix
            Matrix* translation_matrix_2 = construct_translation_matrix(scene->objects[i]->location->x, scene->objects[i]->location->y, scene->objects[i]->location->z);
            Matrix* translation_matrix_1 = construct_translation_matrix(dx, dy, dz);
            Matrix* rotation_matrix_2 = construct_rotation_matrix(-scene->camera->rotation[0], -scene->camera->rotation[1], -scene->camera->rotation[2]);
            Matrix* rotation_matrix_1 = construct_rotation_matrix(scene->objects[i]->rotation[0], scene->objects[i]->rotation[1], scene->objects[i]->rotation[2]);
            // Apply projection matrix onto transformation matrix
            Matrix* RT = matmul(*rotation_matrix_1, *translation_matrix_2);
            Matrix* TRT = matmul(*translation_matrix_1, *RT);
            Matrix* RTRT = matmul(*rotation_matrix_2, *TRT);
            Matrix* augmentation_matrix = matmul(*projection_matrix, *RTRT);
            for (int v = 0; v < scene->objects[i]->model->vertex_count; v++) {
                // Apply augmentation matrix onto vertex to map (x, y, z) of model to (x, y) of screen
                Matrix* point = construct_affine_template_of_coordinate(*scene->objects[i]->model->vertices[v]);
                Matrix* final = matmul(*augmentation_matrix, *point);
                // Perform 1/z calculation
                scalar_multiply_inplace(final, 1/final->values[2]);
                // Write to image2d buffer
                Vertex3D* coord = strip_coords_from_matrix(final);
                printf("%ld: Vertex at %.2f, %.2f\n", current_vertex, coord->x, coord->y); 
                image_buffer->vertices[current_vertex]->x = coord->x;
                image_buffer->vertices[current_vertex]->y = coord->y;
                current_vertex++;
                // Cleanup matrix
                destroy_vertex3D_struct(coord);
                destroy_matrix(point);
                destroy_matrix(final);
            }

            // Cleaning up
            destroy_matrix(translation_matrix_1);
            destroy_matrix(translation_matrix_2);
            destroy_matrix(rotation_matrix_1);
            destroy_matrix(rotation_matrix_2);
            destroy_matrix(RT);
            destroy_matrix(TRT);
            destroy_matrix(RTRT);
            destroy_matrix(augmentation_matrix);
        }
        destroy_matrix(projection_matrix);

        for (int i = 0; i < h*w; i++) {
            int x = i % w;
            int y = (i - x) / w;
            
            x = x;
            y = y;
            bool flag = false;
            for (int j = 0; j < image_buffer->vertex_count; j++) {
                double dx = image_buffer->vertices[j]->x - x;
                double dy = image_buffer->vertices[j]->y - y;
                if (dx*dx + dy*dy < 10)
                    flag = true;
            }
            for (int j = 0; j < image_buffer->triangle_count; j++) {
                Vertex2D v1 = *image_buffer->vertices[image_buffer->triangles[j]->indices[0]];
                Vertex2D v2 = *image_buffer->vertices[image_buffer->triangles[j]->indices[1]];
                Vertex2D v3 = *image_buffer->vertices[image_buffer->triangles[j]->indices[2]];

                double m1 = (v1.y - v2.y) / (v1.x - v2.x);
                double b1 = v1.y - m1*v1.x;
                double dy = y - (x*m1 + b1);
                if (dy*dy < 2 && ((x < v1.x && x > v2.x) || (x > v1.x && x < v2.x)))
                    flag = true;
                
                double m11 = (v1.y - v3.y) / (v1.x - v3.x);
                double b11 = v1.y - m11*v1.x;
                double dy1 = y - (x*m11 + b11);
                if (dy1*dy1 < 2 && ((x < v1.x && x > v3.x) || (x > v1.x && x < v3.x)))
                    flag = true;

                double m111 = (v3.y - v2.y) / (v3.x - v2.x);
                double b111 = v3.y - m111*v3.x;
                double dy11 = y - (x*m111 + b111);
                if (dy11*dy11 < 2 && ((x < v3.x && x > v2.x) || (x > v3.x && x < v2.x)))
                    flag = true;
            }

            framebuffer[i] = SDL_MapRGBA(format, 255, 255, 255, 255);
            if (flag)
                framebuffer[i] = SDL_MapRGBA(format, 0, 0, 0, 255);
        }

        // Perform clipping
        // [IN DEVELOPMENT - NOT IMPLEMENTED YET]

        // Apply vertex shading
        // [IN DEVELOPMENT - NOT IMPLEMENTED YET]

        // Rasterize into fragments
        // [IN DEVELOPMENT - NOT IMPLEMENTED YET]

        // Apply textures onto fragments
        // [IN DEVELOPMENT - NOT IMPLEMENTED YET]

        // Apply shading onto fragments
        // [IN DEVELOPMENT - NOT IMPLEMENTED YET]

        // Clear the screen with a white background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
        SDL_RenderClear(renderer);
        SDL_UpdateTexture(texture, NULL, framebuffer, w * sizeof(Uint32));
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Free resources and close SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_DestroyTexture(texture);
    SDL_FreeFormat(format);
    free(framebuffer);
    destroy_scene_recursively(scene);
    destoy_image_struct(image_buffer);

    return 0;
}
