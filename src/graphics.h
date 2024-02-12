#ifndef SRC_GRAPHICS_H_
#define SRC_GRAPHICS_H_

#include "SDL2/SDL.h"

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} Graphics;

int GraphicsInit(Graphics *graphics);
void GraphicsQuit(Graphics *graphics);

extern Graphics kGraphics;

#endif  /* SRC_GRAPHICS_H_ */
