#include <stdio.h>

#include "SDL2/SDL.h"

#include "src/constants.h"
#include "src/graphics.h"

int GraphicsInit(void) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }

  kGraphics.window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_SHOWN);
  if (kGraphics.window == NULL) {
    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    return 1;
  }

  kGraphics.renderer = SDL_CreateRenderer(kGraphics.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (kGraphics.renderer == NULL) {
    fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
    return 1;
  }

  return 0;
}

void GraphicsQuit(void) {
  SDL_DestroyRenderer(kGraphics.renderer);
  SDL_DestroyWindow(kGraphics.window);
  SDL_Quit();
}

void GraphicsDrawRect(SDL_Rect rect, SDL_Color color) {
  SDL_SetRenderDrawColor(kGraphics.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(kGraphics.renderer, &rect);
}
