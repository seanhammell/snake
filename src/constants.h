#ifndef SNAKE_CONSTANTS_H
#define SNAKE_CONSTANTS_H

#define GRID_SIZE   6
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

struct search_info {
    int steps;
    int length;
    int nodes;
    int completed;
    int total;
};

extern struct search_info search_info;

#endif  /* SNAKE_CONSTANTS_H */
