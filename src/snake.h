#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#define GRID_SIZE 32

#define EATING_APPLE(self, apple)   (self->body[0].x == apple->x && self->body[0].y == apple->y)

enum directions {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    N_DIRECTIONS,
};

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
void snake_destroy(struct snake *self);

void snake_move(struct snake *self);
int snake_biting_tail(struct snake *self);
void snake_grow(struct snake *self);

#endif  /* SNAKE_SNAKE_H */
