#ifndef SRC_GRAPHICS_H_
#define SRC_GRAPHICS_H_

#include "SDL2/SDL.h"

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} Graphics;

int GraphicsInit(void);
void GraphicsQuit(void);

void GraphicsDrawRect(SDL_Rect rect, SDL_Color color);

extern Graphics kGraphics;

#endif  /* SRC_GRAPHICS_H_ */
