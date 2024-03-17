#ifndef SNAKE_CONSTANTS_H
#define SNAKE_CONSTANTS_H

#define GRID_SIZE   6
#define N_CELLS     (GRID_SIZE * GRID_SIZE)

#define TILE_SIZE   16
#define SCREEN_SIZE (GRID_SIZE * TILE_SIZE)

#define IN_BOUNDS(x, y) (x > -1 && y > -1 && x < GRID_SIZE && y < GRID_SIZE)
#define OCCUPIED(s)\
    uint8_t occupied[GRID_SIZE][GRID_SIZE] = {0};\
    for (int i = 0; i < s->length - 1; ++i)\
        occupied[s->body[i].x][s->body[i].y] = 1;\
    occupied[s->body[s->length - 1].x][s->body[s->length - 1].y] = s->length < 3;

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
