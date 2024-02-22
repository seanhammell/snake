#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#define GRID_SIZE   32

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

    struct vec2 chain[GRID_SIZE * GRID_SIZE];
    int links;
};

void game_loop(void);

#endif  /* SNAKE_GAME_H */
