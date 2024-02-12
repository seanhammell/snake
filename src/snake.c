#include <stdlib.h>

#include "src/constants.h"

#ifndef SNAKE_
#define SNAKE_

typedef struct {
  int body[SIZE * SIZE];
  int length;
  Directions direction;
} Snake;

#endif  /* SNAKE_ */

#include "src/snake.h"

Snake *SnakeCreate(void) {
  Snake *snake = malloc(sizeof(Snake));
  snake->body[0] = (SIZE * SIZE) / 2;
  snake->length = 1;
  snake->direction = NONE;
  return snake;
}

void SnakeDestroy(Snake *snake) {
  free(snake);
}
