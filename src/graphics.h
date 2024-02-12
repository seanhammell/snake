#ifndef SRC_GRAPHICS_H_
#define SRC_GRAPHICS_H_

#include "SDL2/SDL.h"

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} Graphics;

int graphics_init(void);
void graphics_quit(void);

extern Graphics graphics;

#endif  /* SRC_GRAPHICS_H_ */
