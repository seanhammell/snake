#ifndef SNAKE_CONSTANTS_H
#define SNAKE_CONSTANTS_H

#define GRID_SIZE   8
#define N_CELLS     (GRID_SIZE * GRID_SIZE)

#define GRAPH_SIZE  (GRID_SIZE / 2)
#define N_VERTICES  (GRAPH_SIZE * GRAPH_SIZE)

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

static const int graph_offsets[N_DIRECTIONS] = {-GRAPH_SIZE, GRAPH_SIZE, -1, 1};

#endif  /* SNAKE_CONSTANTS_H */
