#include "src/graphics.h"

#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"

#include "src/constants.h"

/**
 * Initializes SDL and creates the window and renderer.
 */
struct graphics *graphics_create(void)
{
    struct graphics *graphics = malloc(sizeof(struct graphics));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        graphics_destroy(graphics);
        return NULL;
    }

    graphics->window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_SHOWN);
    if (graphics->window == NULL) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        graphics_destroy(graphics);
        return NULL;
    }

    graphics->renderer = SDL_CreateRenderer(graphics->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (graphics->renderer == NULL) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        graphics_destroy(graphics);
        return NULL;
    }

    return graphics;
}

/**
 * Frees graphics resources.
 */
void graphics_destroy(struct graphics *graphics)
{
    SDL_DestroyRenderer(graphics->renderer);
    SDL_DestroyWindow(graphics->window);
    SDL_Quit();
    free(graphics);
}
