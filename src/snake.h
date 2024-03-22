#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include "src/constants.h"

#define STANDARD_GROWTH     0
#define FORCE_GROWTH        1
#define FULL_SNAKE(self)    (self->length == N_CELLS - 1)

struct snake {
    struct vec2 body[N_CELLS];
    int length;
    int direction;
    struct vec2 apple;
    int steps_since_apple;
};

struct snake *snake_create(void);
struct snake *snake_copy(struct snake *self);
void snake_destroy(struct snake *self);

int snake_generate_moves(struct snake *self, int moves[N_DIRECTIONS]);
int snake_move(struct snake *self, int force_growth);
int snake_biting_tail(struct snake *self);

#endif  /* SNAKE_SNAKE_H */
