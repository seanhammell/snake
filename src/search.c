#include "src/search.h"

#include <stdint.h>
#include <stdlib.h>

#include "src/constants.h"
#include "src/queue.h"

#define FOUND   0

/**
 * Determines if a path is safe by checking if the snake can see its tail.
 */
int safe_path(struct snake *snake)
{
    if (snake->length < 4)
        return 1;

    struct priority_queue *queue = queue_create();
    struct node_args args = {snake->body[0], snake->body[snake->length - 1], 0};
    queue_enqueue(queue, &args);

    OCCUPIED(snake);

    struct node *current;
    while (queue->head < queue->tail) {
        ++search_info.nodes;
        current = queue_dequeue(queue);

        if (VEC2_MATCH(current->position, args.goal)) {
            queue_destroy(queue);
            return 1;
        }

        args.reachable = current->reachable + 1;
        for (int d = 0; d < N_DIRECTIONS; ++d) {
            args.position.x = current->position.x + offsets[d].x;
            args.position.y = current->position.y + offsets[d].y;
            if (IN_BOUNDS(args.position.x, args.position.y) && !occupied[args.position.x][args.position.y]) {
                queue_enqueue(queue, &args);
                occupied[args.position.x][args.position.y] = 1;
            }
        }
    }

    queue_destroy(queue);
    return 0;
}

/**
 * Simulates the possible moves from the current position and returns the cost
 * closest to the goal.
 */
int simulate(struct snake *snake, int bound, int reachable)
{
    int cost = reachable + MANHATTAN(snake->body[0], snake->apple);
    if (cost > bound)
        return cost;

    int moves[N_DIRECTIONS];
    int n_moves = snake_generate_moves(snake, moves);
    int min = N_CELLS;
    for (int i = 0; i < n_moves; ++i) {
        ++search_info.nodes;
        struct snake *copy = snake_copy(snake);
        copy->direction = moves[i];
        if ((snake_move(copy, 1) && safe_path(copy)) || (cost = simulate(copy, bound, reachable + 1)) == FOUND) {
            snake_destroy(copy);
            return FOUND;
        }

        if (cost < min)
            min = cost;

        snake_destroy(copy);
    }

    return min;
}

/**
 * Updates the snake's direction based on the pathfinding algorithm.
 */
void search_pathfinder(struct snake *snake)
{
    int moves[N_DIRECTIONS];
    int n_moves = snake_generate_moves(snake, moves);
    int bound = MANHATTAN(snake->body[0], snake->apple);
    int cost;
    for (;;) {
        for (int i = 0; i < n_moves; ++i) {
            ++search_info.nodes;
            struct snake *copy = snake_copy(snake);
            copy->direction = moves[i];
            if ((snake_move(copy, 1) && safe_path(copy)) || (cost = simulate(copy, bound, 1)) == FOUND) {
                snake->direction = moves[i];
                snake_destroy(copy);
                return;
            }

            snake_destroy(copy);
        }

        bound = cost;
    }
}
