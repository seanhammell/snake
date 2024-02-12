#include "SDL2/SDL.h"

#include "src/constants.h"
#include "src/graphics.h"
#include "src/snake.h"

Graphics graphics;

void game_loop(void) {
  if (graphics_init() != 0) {
    return;
  }

  Snake snake;
  snake.body[0] = 255;
  snake.body[1] = 254;
  snake.body[2] = 238;
  snake.body[3] = 237;
  snake.body[4] = 221;
  snake.length = 5;
  snake.direction = NONE;

  SDL_Event e;
  for (;;) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        return;
      }
    }

    SDL_SetRenderDrawColor(graphics.renderer, 0x11, 0x11, 0x11, 0xFF);
    SDL_RenderClear(graphics.renderer);
    snake_render(&snake);
    SDL_RenderPresent(graphics.renderer);
  }
}

int main(void) {
  game_loop();
  graphics_quit();
  return 0;
}
