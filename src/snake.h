#ifndef SRC_SNAKE_H_
#define SRC_SNAKE_H_

#include <stdint.h>

#include "SDL2/SDL.h"

#include "src/constants.h"

typedef struct {
  int x;
  int y;
  int dx;
  int dy;
} Segment;

typedef struct {
  Segment body[GRID_SIZE * GRID_SIZE];
  int length;
  int direction;
  int offset;
} Snake;

void snake_init(Snake *snake);
void snake_input(Snake *snake, SDL_Event *e);
void snake_update(Snake *snake, uint64_t dt);
void snake_render(Snake *snake);

#endif  /* SRC_SNAKE_H_ */
