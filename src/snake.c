#include "src/snake.h"

#include <stdint.h>
#include <stdlib.h>

#include "src/constants.h"
#include "src/hamiltonian.h"
#include "src/search.h"

#define EATING_APPLE(self, apple)   (self->body[0].x == apple->x && self->body[0].y == apple->y)

/**
 * Increases the snake's length by one.
 */
static void grow(struct snake *self)
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
static void random_apple(struct snake *self)
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
    self->steps_since_apple = 0;
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
    copy->steps_since_apple = self->steps_since_apple;
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
 * Generates the snake's possible moves in ascending order of their distance
 * from the current position on the hamiltonian cycle.
 */
int snake_generate_moves(struct snake *self, int moves[N_DIRECTIONS])
{
    OCCUPIED(self);

    int n_moves = 0;
    int head_index = hamiltonian_index(&self->body[0]);

    struct vec2 step;
    int step_index;
    int step_values[N_DIRECTIONS];
    for (int d = 0; d < N_DIRECTIONS; ++d) {
        step.x = self->body[0].x + offsets[d].x;
        step.y = self->body[0].y + offsets[d].y;
        if (IN_BOUNDS(step.x, step.y) && !occupied[step.x][step.y]) {
            step_index = hamiltonian_index(&step);
            if (step_index < head_index)
                step_index += N_CELLS;

            int value = step_index - head_index;
            int i = 0;
            for (; i < n_moves; ++i) {
                if (value < step_values[i])
                    break;
            }

            for (int j = n_moves; j > i; --j) {
                moves[j] = moves[j - 1];
                step_values[j] = step_values[j - 1];
            }

            moves[i] = d;
            step_values[i] = value;
            ++n_moves;
        }
    }

    return n_moves;
}

/**
 * Moves the snake one step in the direction it is facing and return if the
 * move resulted in the snake eating the apple.
 */
int snake_move(struct snake *self, int force_growth)
{
    if (self->direction < N_DIRECTIONS) {
        struct vec2 step;
        step.x = self->body[0].x + offsets[self->direction].x;
        step.y = self->body[0].y + offsets[self->direction].y;
        if (VEC2_MATCH(step, self->apple) || force_growth)
            grow(self);

        for (int i = self->length - 1; i > 0; --i)
            self->body[i] = self->body[i - 1];

        self->body[0].x += offsets[self->direction].x;
        self->body[0].y += offsets[self->direction].y;

        if (VEC2_MATCH(step, self->apple)) {
            self->steps_since_apple = 0;
            random_apple(self);
            return 1;
        }

        ++self->steps_since_apple;
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
