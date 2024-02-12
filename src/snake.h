#ifndef SRC_SNAKE_H_
#define SRC_SNAKE_H_

#include "src/constants.h"

typedef struct {
  int body[GRID_SIZE * GRID_SIZE];
  int length;
  Directions direction;
} Snake;

void snake_render(Snake *self);

#endif  /* SRC_SNAKE_H_ */
