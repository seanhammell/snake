#include "SDL2/SDL.h"

#include "src/graphics.h"
#include "src/snake.h"

Graphics kGraphics;

void GameLoop(void) {
  Snake *snake = SnakeCreate();

  SDL_Event e;
  for (;;) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        SnakeDestroy(snake);
        return;
      }
    }

    SDL_SetRenderDrawColor(kGraphics.renderer, 0x11, 0x11, 0x11, 0xFF);
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
