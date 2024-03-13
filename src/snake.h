#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include "src/constants.h"

#define EATING_APPLE(self, apple)   (self->body[0].x == apple->x && self->body[0].y == apple->y)
#define FULL_SNAKE(self)            (self->length == GRID_SIZE * GRID_SIZE - 1)

struct vec2 {
    int x;
    int y;
};

struct snake {
    struct vec2 body[GRID_SIZE * GRID_SIZE];
    int length;
    int direction;
};

struct snake *snake_create(void);
struct snake *snake_copy(struct snake *self);
void snake_destroy(struct snake *self);

void snake_move(struct snake *self);
int snake_biting_tail(struct snake *self);
void snake_grow(struct snake *self);

#endif  /* SNAKE_SNAKE_H */
