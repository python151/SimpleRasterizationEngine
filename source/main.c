#include <SDL2/SDL.h>

#include "STL_Handler.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    /* Initializes SDL Stuff */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Rasterizer",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800, 600,
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


    // Load models into memory
    // [IN DEVELOPMENT - NOT IMPLEMENTED YET]

    // Construct a scene from these
    // [IN DEVELOPMENT - NOT IMPLEMENTED YET]

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

        // Transform into Camera space
        // [IN DEVELOPMENT - NOT IMPLEMENTED YET]

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

        // Render framebuffer to screne
        // [IN DEVELOPMENT - NOT IMPLEMENTED YET]


        // Clear the screen with a white background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
        SDL_RenderClear(renderer);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Free resources and close SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
