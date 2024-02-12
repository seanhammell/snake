#include "SDL2/SDL.h"

#include "src/graphics.h"

Graphics kGraphics;

void GameLoop(void) {
  SDL_Event e;
  for (;;) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        return;
      }
    }

    SDL_SetRenderDrawColor(kGraphics.renderer, 0xB5, 0xA3, 0x7E, 0xFF);
    SDL_RenderClear(kGraphics.renderer);
    SDL_RenderPresent(kGraphics.renderer);
  }
}

int main(void) {
  if (GraphicsInit(&kGraphics) == 0) {
    GameLoop();
  }

  GraphicsQuit(&kGraphics);
  return 0;
}
