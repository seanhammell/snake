#include "SDL2/SDL.h"

#include "src/graphics.h"
#include "src/snake.h"

Graphics graphics;

void game_loop(void)
{
  if (graphics_init() != 0) {
    return;
  }

  Snake snake;
  snake_init(&snake);

  SDL_Event e;
  uint64_t timer = 0;
  for (;;) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        return;
      }

      snake_input(&snake, &e);
    }

    uint64_t now = SDL_GetTicks64();
    snake_update(&snake, now - timer);
    timer = now;

    SDL_SetRenderDrawColor(graphics.renderer, 0x11, 0x11, 0x11, 0xFF);
    SDL_RenderClear(graphics.renderer);
    snake_render(&snake);
    SDL_RenderPresent(graphics.renderer);
  }
}

int main(void)
{
  game_loop();
  graphics_quit();
  return 0;
}
