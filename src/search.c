#include "src/search.h"

#include <stdint.h>
#include <stdlib.h>

#include "src/constants.h"

#define MANHATTAN(a, b) (abs(a.x - b.x) + abs(a.y - b.y))
#define IN_BOUNDS(x, y) (x > -1 && y > -1 && x < GRID_SIZE && y < GRID_SIZE)

#define OCCUPIED\
    uint8_t occupied[GRID_SIZE][GRID_SIZE] = {0};\
    for (int i = 0; i < snake->length - 1; ++i)\
        occupied[snake->body[i].x][snake->body[i].y] = 1;\
    occupied[snake->body[snake->length - 1].x][snake->body[snake->length - 1].y] = snake->length < 3;

struct queue {
    struct vec2 queue[N_CELLS];
    int head;
    int tail;
};

int safe_path(struct snake *snake)
{
    if (snake->length < 4)
        return 1;

    OCCUPIED;

    struct queue queue;
    queue.head = 0;
    queue.tail = 0;

    queue.queue[queue.tail].x = snake->body[0].x;
    queue.queue[queue.tail].y = snake->body[0].y;
    ++queue.tail;

    struct vec2 current;
    while (queue.head < queue.tail) {
        ++search_info.nodes;
        current.x = queue.queue[queue.head].x;
        current.y = queue.queue[queue.head].y;

        if (current.x == snake->body[snake->length - 1].x && current.y == snake->body[snake->length - 1].y)
            return 1;

        ++queue.head;
        occupied[current.x][current.y] = 1;
        for (int d = 0; d < N_DIRECTIONS; ++d) {
            queue.queue[queue.tail].x = current.x + offsets[d].x;
            queue.queue[queue.tail].y = current.y + offsets[d].y;
            int in_queue = 0;
            for (int i = queue.head; i < queue.tail; ++i) {
                if (queue.queue[queue.tail].x == queue.queue[i].x && queue.queue[queue.tail].y == queue.queue[i].y) {
                    in_queue = 1;
                    break;
                }
            }

            if (IN_BOUNDS(queue.queue[queue.tail].x, queue.queue[queue.tail].y) && !occupied[queue.queue[queue.tail].x][queue.queue[queue.tail].y] && !in_queue)
                ++queue.tail;
        }
    }

    return 0;
}

int simulate(struct snake *snake, int depth)
{
    if (depth == 0)
        return 0;

    OCCUPIED;
    struct vec2 step;
    for (int d = 0; d < N_DIRECTIONS; ++d) {
        ++search_info.nodes;
        step.x = snake->body[0].x + offsets[d].x;
        step.y = snake->body[0].y + offsets[d].y;
        if (IN_BOUNDS(step.x, step.y) && !occupied[step.x][step.y]) {
            struct snake *copy = snake_copy(snake);
            copy->direction = d;
            if ((snake_move(copy) && safe_path(copy)) || simulate(copy, depth - 1)) {
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
    OCCUPIED;
    struct vec2 step;
    int base = MANHATTAN(snake->body[0], snake->apple);
    for (int depth = base;; ++depth) {
        for (int d = 0; d < N_DIRECTIONS; ++d) {
            ++search_info.nodes;
            step.x = snake->body[0].x + offsets[d].x;
            step.y = snake->body[0].y + offsets[d].y;
            if (IN_BOUNDS(step.x, step.y) && !occupied[step.x][step.y]) {
                struct snake *copy = snake_copy(snake);
                copy->direction = d;
                if ((snake_move(copy) && safe_path(copy)) || simulate(copy, depth - 1)) {
                    snake->direction = d;
                    snake_destroy(copy);
                    return;
                }

                snake_destroy(copy);
            }
        }
    }
}
