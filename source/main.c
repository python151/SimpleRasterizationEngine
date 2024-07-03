#include <SDL2/SDL.h>

#include "STL_Handler.h"
#include "Scene.h"
#include "Rasterizer.h"

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

    // List of pointers to each GameObject, will get free'd when we call destroy_scene_recursively()
    GameObject** objects = malloc(sizeof(GameObject*));

    // Construct a scene from these
    Vertex3D* p = build_vertex3D_struct(0, 0, 0);
    Camera* camera = build_camera_struct(p, 0, 0, 0, 60);
    Scene* scene = build_scene_struct(camera, objects, 1);

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

        // Update scene
        // [IN DEVELOPMENT - NOT IMPLEMENTED YET]

        // Construct projection matrix
        // For each model
            // Find total delta (translation, rotation, etc. to go from model space into world space and then with respect to the camera)
            // Construct transformation matrix
            // Apply projection matrix onto transformation matrix
            // For each vertex
                // Apply projection/transformation matrix onto vertex to map (x, y, z) of model to (x, y) of screen
                // Write to image2d buffer

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

    return 0;
}
