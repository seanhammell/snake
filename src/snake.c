#include <stdlib.h>

#include "SDL2/SDL.h"

#include "src/constants.h"
#include "src/graphics.h"

#ifndef SNAKE_
#define SNAKE_

typedef struct {
  int body[GRID_SIZE * GRID_SIZE];
  int length;
  Directions direction;
} Snake;

#endif  /* SNAKE_ */

#include "src/snake.h"

#define X(self, i) ((self->body[i] % TILE_SIZE) * TILE_SIZE)
#define Y(self, i) ((self->body[i] / TILE_SIZE) * TILE_SIZE)

Snake *SnakeCreate(void) {
  Snake *self = malloc(sizeof(Snake));
  self->body[0] = 255;
  self->body[1] = 254;
  self->body[2] = 253;
  self->body[3] = 237;
  self->body[4] = 221;
  self->length = 5;
  self->direction = NONE;
  return self;
}

void SnakeDestroy(Snake *self) {
  free(self);
}

void SnakeRender(Snake *self) {
  static const SDL_Color kSnakeColor = {0x40, 0x98, 0x5E, 0xFF};
  for (int i = 0; i < self->length; ++i) {
    SDL_Rect segment = {X(self, i), Y(self, i), 14, 14};
    GraphicsDrawRect(segment, kSnakeColor);
  }
}
