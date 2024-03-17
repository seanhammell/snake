#include "src/search.h"

#include <stdint.h>
#include <stdlib.h>

#include "src/constants.h"

struct queue {
    struct vec2 queue[N_CELLS];
    int head;
    int tail;
};

int safe_path(struct snake *snake)
{
    if (snake->length < 4)
        return 1;

    OCCUPIED(snake);

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

    int moves[N_DIRECTIONS];
    int n_moves = snake_generate_moves(snake, moves);
    for (int i = 0; i < n_moves; ++i) {
        ++search_info.nodes;
        struct snake *copy = snake_copy(snake);
        copy->direction = moves[i];
        if ((snake_move(copy) && safe_path(copy)) || simulate(copy, depth - 1)) {
            snake_destroy(copy);
            return 1;
        }

        snake_destroy(copy);
    }

    return 0;
}

/**
 * Updates the snake's direction based on the pathfinding algorithm.
 */
void search_pathfinder(struct snake *snake)
{
    int base = MANHATTAN(snake->body[0], snake->apple);
    int moves[N_DIRECTIONS];
    int n_moves = snake_generate_moves(snake, moves);

    for (int depth = base;; ++depth) {
        for (int i = 0; i < n_moves; ++i) {
            ++search_info.nodes;
            struct snake *copy = snake_copy(snake);
            copy->direction = moves[i];
            if ((snake_move(copy) && safe_path(copy)) || simulate(copy, depth - 1)) {
                snake->direction = moves[i];
                snake_destroy(copy);
                return;
            }

            snake_destroy(copy);
        }
    }
}
