#include "src/search.h"

#include <stdint.h>
#include <stdlib.h>

#include "src/constants.h"

#define MANHATTAN(a, b) (abs(a.x - b.x) + abs(a.y - b.y))
#define IN_BOUNDS(x, y) (x > -1 && y > -1 && x < GRID_SIZE && y < GRID_SIZE)

int simulate(struct snake *snake, int depth)
{
    if (depth == 0)
        return 0;

    uint8_t occupied[GRID_SIZE][GRID_SIZE] = {0};
    for (int i = 0; i < snake->length; ++i)
        occupied[snake->body[i].x][snake->body[i].y] = 1;

    struct vec2 step;
    for (int d = 0; d < N_DIRECTIONS; ++d) {
        step.x = snake->body[0].x + offsets[d].x;
        step.y = snake->body[0].y + offsets[d].y;
        if (IN_BOUNDS(step.x, step.y) && !occupied[step.x][step.y]) {
            struct snake *copy = snake_copy(snake);
            copy->direction = d;
            if (snake_move(copy) || simulate(copy, depth - 1)) {
                snake->direction = d;
                snake_destroy(copy);
                return 1;
            }

            snake_destroy(copy);
        }
    }

    return 0;
}

/**
 * Updates the snake's direction based on the pathfinding algorithm.
 */
void search_pathfinder(struct snake *snake)
{
    uint8_t occupied[GRID_SIZE][GRID_SIZE] = {0};
    for (int i = 0; i < snake->length; ++i)
        occupied[snake->body[i].x][snake->body[i].y] = 1;

    struct vec2 step;
    int base = MANHATTAN(snake->body[0], snake->apple);
    for (int depth = base; depth < N_CELLS; ++depth) {
        for (int d = 0; d < N_DIRECTIONS; ++d) {
            step.x = snake->body[0].x + offsets[d].x;
            step.y = snake->body[0].y + offsets[d].y;
            if (IN_BOUNDS(step.x, step.y) && !occupied[step.x][step.y]) {
                struct snake *copy = snake_copy(snake);
                copy->direction = d;
                if (snake_move(copy) || simulate(copy, depth - 1)) {
                    snake->direction = d;
                    snake_destroy(copy);
                    return;
                }

                snake_destroy(copy);
            }
        }
    }
}
