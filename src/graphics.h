#ifndef SNAKE_GRAPHICS_H
#define SNAKE_GRAPHICS_H

#include "SDL2/SDL.h"

struct graphics {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

struct graphics *graphics_create(void);
void graphics_destroy(struct graphics *graphics);

#endif  /* SNAKE_GRAPHICS_H */
