#include "src/snake.h"

#include <stdint.h>

#include "SDL2/SDL.h"

#include "src/constants.h"
#include "src/graphics.h"

enum directions {
  STOP,
  UP,
  DOWN,
  LEFT,
  RIGHT,
};

void snake_init(Snake *snake)
{
  snake->body[0].x = GRID_SIZE / 2;
  snake->body[0].y = GRID_SIZE / 2;
  snake->body[0].dx = 0;
  snake->body[0].dy = 0;
  snake->length = 1;
  snake->direction = STOP;
}

void snake_input(Snake *snake, SDL_Event *e)
{
  if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
      case SDLK_UP:
        if (snake->direction != DOWN) {
          snake->direction = UP;
        }
        break;
      case SDLK_DOWN:
        if (snake->direction != UP) {
          snake->direction = DOWN;
        }
        break;
      case SDLK_LEFT:
        if (snake->direction != RIGHT) {
          snake->direction = LEFT;
        }
        break;
      case SDLK_RIGHT:
        if (snake->direction != LEFT) {
          snake->direction = RIGHT;
        }
        break;
      default:
        break;
    }
  }
}

void snake_update(Snake *snake, uint64_t dt)
{
  static const uint64_t interval = 80;
  static uint64_t elapsed = 0;
  elapsed += dt;
  snake->offset = elapsed / (interval / TILE_SIZE);
  while (elapsed > interval) {
    elapsed -= interval;
    snake->offset = 0;
    for (int i = snake->length - 1; i > 0; --i) {
      snake->body[i] = snake->body[i - 1];
    }

    snake->body[0].x += snake->body[0].dx;
    snake->body[0].y += snake->body[0].dy;
    switch (snake->direction) {
      case UP:
        snake->body[0].dx = 0;
        snake->body[0].dy = -1;
        break;
      case DOWN:
        snake->body[0].dx = 0;
        snake->body[0].dy = 1;
        break;
      case LEFT:
        snake->body[0].dx = -1;
        snake->body[0].dy = 0;
        break;
      case RIGHT:
        snake->body[0].dx = 1;
        snake->body[0].dy = 0;
        break;
    }
  }
}

void snake_render(Snake *snake)
{
  SDL_Rect segment = {0, 0, 14, 14};
  for (int i = 0; i < snake->length; ++i) {
    segment.x = (snake->body[i].x * TILE_SIZE) + (snake->offset * snake->body[i].dx);
    segment.y = (snake->body[i].y * TILE_SIZE) + (snake->offset * snake->body[i].dy);
    SDL_SetRenderDrawColor(graphics.renderer, 0x40, 0x98, 0x5E, 0xFF);
    SDL_RenderFillRect(graphics.renderer, &segment);
  }
}
