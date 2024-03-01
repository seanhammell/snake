#include "src/search.h"

#include <stdint.h>
#include <stdio.h>

#include "src/snake.h"
#include "src/queue.h"

#define LEFT_COLUMN(x, y)   (x == 0 && y < GRID_SIZE - 1)
#define ODD_ROW(x, y)       (y % 2)
#define EVEN_ROW(x, y)      (!ODD_ROW(x, y))
#define END_OF_ROW(x, y)    ((x == 1 && EVEN_ROW(x, y) && y > 0) || (x == GRID_SIZE - 1 && ODD_ROW(x, y)))
#define IN_BOUNDS(x, y)     (x > -1 && y > -1 && x < GRID_SIZE && y < GRID_SIZE)

static const struct vec2 offsets[N_DIRECTIONS] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

static void enqueue_moves(struct priority_queue *queue, struct node *node, struct vec2 *apple, uint8_t seen[GRID_SIZE][GRID_SIZE])
{
    struct node_args args;
    args.reachable = node->reachable + 1;
    args.goal = apple;

    for (int i = 0; i < N_DIRECTIONS; ++i) {
        args.position.x = node->position.x + offsets[i].x;
        args.position.y = node->position.y + offsets[i].y;
        if (IN_BOUNDS(args.position.x, args.position.y) && !seen[args.position.x][args.position.y]) {
            queue_enqueue(queue, &args);
            seen[args.position.x][args.position.y] = 1;
        }
    }
}

static void hamiltonian_cycle(struct snake *snake)
{
    int x = snake->body[0].x;
    int y = snake->body[0].y;
    if (LEFT_COLUMN(x, y))
        snake->direction = DOWN;
    else if (END_OF_ROW(x, y))
        snake->direction = UP;
    else if (EVEN_ROW(x, y))
        snake->direction = LEFT;
    else if (ODD_ROW(x, y))
        snake->direction = RIGHT;
}

static int a_star(struct snake *snake, struct vec2 *apple)
{
    uint8_t seen[GRID_SIZE][GRID_SIZE] = {0};
    for (int i = 0; i < snake->length; ++i)
        seen[snake->body[i].x][snake->body[i].y] = 1;

    struct priority_queue *queue = queue_create();
    struct node_args head_args = {snake->body[0], 0, apple};
    queue_enqueue(queue, &head_args);

    struct node *node;
    int path = GRID_SIZE * GRID_SIZE;
    while (queue->head < queue->tail) {
        node = queue_dequeue(queue);

        if (node->position.x == apple->x && node->position.y == apple->y) {
            path = node->reachable;
            break;
        }

        enqueue_moves(queue, node, apple, seen);
    }

    queue_destroy(queue);
    return path;
}

void search_pathfinder(struct snake *snake, struct vec2 *apple)
{
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
            snake_move(copy);
            int path = a_star(copy, apple);
            switch (d) {
            case UP:
                fprintf(stderr, "UP");
                break;
            case DOWN:
                fprintf(stderr, "DOWN");
                break;
            case LEFT:
                fprintf(stderr, "LEFT");
                break;
            case RIGHT:
                fprintf(stderr, "RIGHT");
                break;
            default:
                break;
            }

            fprintf(stderr, ", %d; ", path);
            snake_destroy(copy);
        }
    }

    fprintf(stderr, "\n");
    hamiltonian_cycle(snake);
}
