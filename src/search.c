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

static void enqueue_moves(struct priority_queue *queue, struct node *node, struct vec2 *apple, uint8_t seen[GRID_SIZE][GRID_SIZE])
{
    static const struct vec2 offsets[N_DIRECTIONS] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

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

void search_pathfinder(struct snake *snake, struct vec2 *apple)
{
    struct priority_queue *queue = queue_create();
    struct node_args head_args = {snake->body[0], 0, apple};
    queue_enqueue(queue, &head_args);

    uint8_t seen[GRID_SIZE][GRID_SIZE] = {0};
    for (int i = 0; i < snake->length; ++i)
        seen[snake->body[i].x][snake->body[i].y] = 1;

    struct node *head = queue_dequeue(queue);
    enqueue_moves(queue, head, apple, seen);

    hamiltonian_cycle(snake);

    queue_destroy(queue);
}
