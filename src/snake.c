#include "src/snake.h"

#include <stdint.h>
#include <stdlib.h>

#include "src/constants.h"
#include "src/search.h"

#define EATING_APPLE(self, apple)   (self->body[0].x == apple->x && self->body[0].y == apple->y)

/**
 * Increases the snake's length by one.
 */
void grow(struct snake *self)
{
    if (self->length + 1 == N_CELLS)
        return;

    self->body[self->length].x = self->body[self->length - 1].x;
    self->body[self->length].y = self->body[self->length - 1].y;
    ++self->length;
}

/**
 * Generates an apple in a random unoccupied position.
 */
void random_apple(struct snake *self)
{
    int occupied[GRID_SIZE][GRID_SIZE] = {0};
    for (int i = 0; i < self->length; ++i)
        occupied[self->body[i].x][self->body[i].y] = 1;

    do {
        self->apple.x = rand() % GRID_SIZE;
        self->apple.y = rand() % GRID_SIZE;
    } while (occupied[self->apple.x][self->apple.y]);
}

/**
 * Creates a new snake.
 */
struct snake *snake_create(void)
{
    struct snake *self = malloc(sizeof(struct snake));
    self->body[0].x = rand() % GRID_SIZE;
    self->body[0].y = rand() % GRID_SIZE;
    self->length = 1;
    self->direction = N_DIRECTIONS;
    random_apple(self);
    return self;
}

/**
 * Makes a copy of the snake.
 */
struct snake *snake_copy(struct snake *self)
{
    struct snake *copy = malloc(sizeof(struct snake));
    for (int i = 0; i < self->length; ++i)
        copy->body[i] = self->body[i];
    copy->length = self->length;
    copy->direction = self->direction;
    copy->apple.x = self->apple.x;
    copy->apple.y = self->apple.y;
    return copy;
}

/**
 * Destorys the snake.
 */
void snake_destroy(struct snake *self)
{
    free(self);
}

/**
 * Moves the snake one step in the direction it is facing and return if the
 * move resulted in the snake eating the apple.
 */
int snake_move(struct snake *self)
{
    if (self->direction < N_DIRECTIONS) {
        struct vec2 step;
        step.x = self->body[0].x + offsets[self->direction].x;
        step.y = self->body[0].y + offsets[self->direction].y;
        if (step.x == self->apple.x && step.y == self->apple.y)
            grow(self);

        for (int i = self->length - 1; i > 0; --i)
            self->body[i] = self->body[i - 1];

        self->body[0].x += offsets[self->direction].x;
        self->body[0].y += offsets[self->direction].y;

        if (step.x == self->apple.x && step.y == self->apple.y) {
            random_apple(self);
            return 1;
        }
    }

    return 0;
}

/**
 * Returns whether the snake is biting its tail.
 */
int snake_biting_tail(struct snake *self)
{
    for (int i = 1; i < self->length; ++i) {
        if (self->body[0].x == self->body[i].x && self->body[0].y == self->body[i].y)
            return 1;
    }

    return 0;
}
