#include "src/snake.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct snake *snake_create(void)
{
    struct snake *self = malloc(sizeof(struct snake));
    self->body[0].x = (GRID_SIZE / 2) - 1;
    self->body[0].y = (GRID_SIZE / 2) - 1;
    self->length = 1;
    self->direction = N_DIRECTIONS;
    return self;
}

void snake_destroy(struct snake *self)
{
    free(self);
}

void snake_move(struct snake *self)
{
    for (int i = self->length - 1; i > 0; --i)
        self->body[i] = self->body[i - 1];

    switch (self->direction) {
        case UP:
            --self->body[0].y;
            break;
        case DOWN:
            ++self->body[0].y;
            break;
        case LEFT:
            --self->body[0].x;
            break;
        case RIGHT:
            ++self->body[0].x;
            break;
    }
}

int snake_biting_tail(struct snake *self)
{
    for (int i = 1; i < self->length; ++i) {
        if (self->body[0].x == self->body[i].x && self->body[0].y == self->body[i].y)
            return 1;
    }

    return 0;
}

void snake_grow(struct snake *self)
{
    if (self->length + 1 == GRID_SIZE * GRID_SIZE)
        return;

    self->body[self->length].x = self->body[self->length - 1].x;
    self->body[self->length].y = self->body[self->length - 1].y;
    ++self->length;
}
