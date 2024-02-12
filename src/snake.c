#include "src/snake.h"

#include <stdlib.h>

#include "SDL2/SDL.h"

#include "src/constants.h"
#include "src/graphics.h"

#define X_POS(i) ((i % TILE_SIZE) * TILE_SIZE)
#define Y_POS(i) ((i / TILE_SIZE) * TILE_SIZE)

void snake_render(Snake *snake) {
  SDL_Rect segment = {0, 0, 14, 14};
  for (int i = 0; i < snake->length; ++i) {
    segment.x = X_POS(snake->body[i]);
    segment.y = Y_POS(snake->body[i]);
    SDL_SetRenderDrawColor(graphics.renderer, 0x40, 0x98, 0x5E, 0xFF);
    SDL_RenderFillRect(graphics.renderer, &segment);
  }
}
