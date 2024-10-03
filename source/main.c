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

void bresenhams_line_algorithm(Uint32* framebuffer, int w, int h, Vertex2D* v1, Vertex2D* v2, Uint32 pixel) {
    int x1 = round(v1->x);
    int y1 = round(v1->y);
    int x2 = round(v2->x);
    int y2 = round(v2->y);
    
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    
    int err = dx - dy;
    
    while (true) {
        int i = y1 * w + x1;
        if (i >= 0 && i < w * h) {
            framebuffer[i] = pixel;
        }

        // Break if we've reached the end point
        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

Uint32 calculate_light_intensity() {}

void flood_fill(Uint32* framebuffer, int w, int h, int x, int y, Uint32 marker, Uint32 fill_color) {
    int* indexes = malloc(sizeof(int) * 1000);
    indexes[0] = w*y + x;
    int len = 1;
    int stack_size = 1000;
    int i = 0;

    while (i < len) {
        x = indexes[i] % w;
        y = (indexes[i] - x) / w;
        i++;

        if (x < 0 || x >= w || y < 0 || y >= h) {
            continue;
        }
        
        if (framebuffer[y * w + x] == marker || framebuffer[y * w + x] == fill_color) {
            continue;
        }

        framebuffer[indexes[i-1]] = fill_color;

        len += 4;
        if (len > stack_size) {
            stack_size *= 2;
            indexes = realloc(indexes, sizeof(int)*stack_size);
        }

        indexes[len-4] = w*(y) + (x + 1);
        indexes[len-3] = w*(y) + (x - 1);
        indexes[len-2] = w*(y + 1) + (x);
        indexes[len-1] = w*(y - 1) + (x);
    }

    free(indexes);
}




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
    object->rotation[0] = 3.14/4;//3.14/10;//3.14/6;
    object->rotation[1] = 0;//3.14/6;
    object->rotation[2] = 3.14/4;//.3;//3.14/6;

    // List of pointers to each GameObject, will get free'd when we call destroy_scene_recursively()
    GameObject** objects = malloc(sizeof(GameObject*));
    objects[0] = object;

    // Construct a scene from these
    puts("Building scene");
    Vertex3D* p = build_vertex3D_struct(0, 70, -250);
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
        object->rotation[1] += .2*deltaTime;
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


        for (int i = 0; i < w*h; i++) {
            framebuffer[i] = SDL_MapRGBA(format, 255, 255, 255, 255);
        }

        for (int i = 0; i < image_buffer->triangle_count; i++) { // image_buffer->triangle_count
            Vertex2D* v1 = image_buffer->vertices[image_buffer->triangles[i]->indices[0]];
            Vertex2D* v2 = image_buffer->vertices[image_buffer->triangles[i]->indices[1]];
            Vertex2D* v3 = image_buffer->vertices[image_buffer->triangles[i]->indices[2]];

            Uint32 marker_color = SDL_MapRGBA(format,i, 128, 128, 255);
            Uint32 fill_color = SDL_MapRGBA(format, i, 128, 128, 255);

            int x_start = (v1->x + v2->x + v3->x) / 3;
            int y_start = (v1->y + v2->y + v3->y) / 3;

            bresenhams_line_algorithm(framebuffer, w, h, v1, v2, marker_color);
            bresenhams_line_algorithm(framebuffer, w, h, v1, v3, marker_color);
            bresenhams_line_algorithm(framebuffer, w, h, v2, v3, marker_color);
            flood_fill(framebuffer, w, h, x_start, y_start, marker_color, fill_color);
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
