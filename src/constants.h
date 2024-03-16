#ifndef SNAKE_CONSTANTS_H
#define SNAKE_CONSTANTS_H

#define GRID_SIZE   4
#define N_CELLS     (GRID_SIZE * GRID_SIZE)

#define TILE_SIZE   16
#define SCREEN_SIZE (GRID_SIZE * TILE_SIZE)

enum directions {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    N_DIRECTIONS,
    STOP,
};

struct vec2 {
    int x;
    int y;
};

static const struct vec2 offsets[N_DIRECTIONS] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

#endif  /* SNAKE_CONSTANTS_H */
