#include "src/search.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#include "SDL2/SDL.h"

#include "src/constants.h"
#include "src/queue.h"

#define FOUND   0

static uint64_t time_limit = 0;

/**
 * Determines if a path is safe by checking if the snake can see its tail. If
 * fill is set, the check continues until the queue is empty, attempting to
 * fill the grid.
 */
static int path_safe(struct snake *snake, int fill)
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

        if (VEC2_MATCH(current->position, args.goal) && !fill) {
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
    if (!fill)
        return 0;

    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = 0; y < GRID_SIZE; ++y) {
            if (!occupied[x][y])
                return 0;
        }
    }

    return 1;
}

/**
 * Returns if any move from the current position can see all unoccupied cells
 * on the grid.
 */
static int can_fill_unoccupied(struct snake *snake)
{
    int moves[N_DIRECTIONS];
    int n_moves = snake_generate_moves(snake, moves);
    for (int i = 0; i < n_moves; ++i) {
        struct snake *copy = snake_copy(snake);
        copy->direction = moves[i];
        snake_move(copy);
        if (path_safe(copy, 1)) {
            snake_destroy(copy);
            return 1;
        }

        snake_destroy(copy);
    }

    return 0;
}

/**
 * Makes the possible moves from the current position and returns if the snake
 * eats the apple, or the cost closest to the goal if the cost exceeds the
 * bound limit.
 */
static int iterative_deepening_astar(struct snake *snake, int bound, int reachable)
{
    if (SDL_GetTicks64() > time_limit)
        return INT_MAX;

    int cost = reachable + MANHATTAN(snake->body[0], snake->apple);
    if (cost > bound)
        return cost;

    int moves[N_DIRECTIONS];
    int n_moves = snake_generate_moves(snake, moves);
    int min = INT_MAX;
    for (int i = 0; i < n_moves; ++i) {
        ++search_info.nodes;
        if (SDL_GetTicks64() > time_limit)
            break;

        struct snake *copy = snake_copy(snake);
        copy->direction = moves[i];
        if ((snake_move(copy) && can_fill_unoccupied(copy)) || (cost = iterative_deepening_astar(copy, bound, reachable + 1)) == FOUND) {
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
 * Attempts to find a path using iterative deepening A*. If no path is found
 * when the depth bound is hit, select the move that moves farthest from the
 * tail without losing sight of the tail.
 */
void search_pathfinder(struct snake *snake)
{
    time_limit = SDL_GetTicks64() + INTERVAL - 2;

    int moves[N_DIRECTIONS];
    int n_moves = snake_generate_moves(snake, moves);

    /* Search for the shortest path to the apple where the head of the snake
    can make a move that has access to all unoccupied cells on the grid after
    eating the apple. */
    int bound, min, cost;
    bound = MANHATTAN(snake->body[0], snake->apple);
    while (SDL_GetTicks64() < time_limit) {
        min = INT_MAX;
        for (int i = 0; i < n_moves; ++i) {
            ++search_info.nodes;
            if (SDL_GetTicks64() > time_limit)
                break;

            struct snake *copy = snake_copy(snake);
            copy->direction = moves[i];
            if ((snake_move(copy) && can_fill_unoccupied(copy)) || (cost = iterative_deepening_astar(copy, bound, 1)) == FOUND) {
                snake->direction = moves[i];
                snake_destroy(copy);
                return;
            }

            if (cost < min)
                min = cost;

            snake_destroy(copy);
        }

        bound = min;
    }

    /* If the snake has made N_CELLS moves since the last time it ate an apple,
    and it didn't return from the iterative deepening A* with a path to the
    apple, it is probably in a loop. Pick a random safe move to try and break
    the loop. */
    if (snake->steps_since_apple > N_CELLS) {
        for (;;) {
            struct snake *copy = snake_copy(snake);
            int random = rand() % n_moves;
            copy->direction = moves[random];
            snake_move(copy);
            if (path_safe(copy, 0)) {
                snake->direction = moves[random];
                snake_destroy(copy);
                return;
            }

            snake_destroy(copy);
        }
    }

    /* If the snake didn't find a path with iterative deepening A* and isn't in
    a loop, pick the move that takes it farthest from its tail without losing
    sight of it. */
    for (int i = 0; i < n_moves; ++i) {
        struct snake *copy = snake_copy(snake);
        copy->direction = moves[i];
        snake_move(copy);
        if (path_safe(copy, 0)) {
            snake->direction = moves[i];
            snake_destroy(copy);
            return;
        }

        snake_destroy(copy);
    }
}
