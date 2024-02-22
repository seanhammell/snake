/**
 * A* Search
 *  Parameters
 *   - initial position
 *   - goal position
 *   - search space
 * 
 *  Return Value
 *   - move to make OR full path
 * 
 *  Data Structures
 *   - node that connects parents and children in the path
 *   - priority queue of unexplored nodes
 * 
 *  Functions
 *   - possible move generator
 *   - manhattan distance calculator
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "src/game.h"
#include "src/search.h"

#define MAX_QUEUE_SIZE  (GRID_SIZE * GRID_SIZE)

#define MANHATTAN(a, b) (abs(a->x - b->x) + abs(a->y - b->y))

#define IN_BOUNDS(x, y) (x > -1 && y > -1 && x < GRID_SIZE && y < GRID_SIZE)

struct node {
    struct node *parent;
    struct vec2 position;
    int reachable;
    int heuristic;
};

struct node_args {
    struct node *parent;
    struct vec2 position;
    struct vec2 *goal;
};

struct priority_queue {
    struct node *queue[MAX_QUEUE_SIZE];
    int head;
    int tail;
};

/**
 * Creates a priority queue and all of the nodes within the queue.
 */
static struct priority_queue *queue_create(void)
{
    struct priority_queue *queue = malloc(sizeof(struct priority_queue));
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i) {
        queue->queue[i] = malloc(sizeof(struct node));
        queue->queue[i]->parent = NULL;
        queue->queue[i]->position.x = 0;
        queue->queue[i]->position.y = 0;
        queue->queue[i]->reachable = 0;
        queue->queue[i]->heuristic = 0;
    }

    queue->head = 0;
    queue->tail = 0;
    return queue;
}

/**
 * Frees all of the memory associated with the priority queue.
 */
static void queue_destroy(struct priority_queue *queue)
{
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i)
        free(queue->queue[i]);
    free(queue);
}

/**
 * Enqueues a node to the priority queue.
 */
static void enqueue(struct priority_queue *queue, struct node_args *args)
{
    if (queue->tail == MAX_QUEUE_SIZE) {
        fprintf(stderr, "Queue full\n");
        return;
    }

    /* Set the members of the new node. */
    struct node *new = queue->queue[queue->tail];
    new->parent = args->parent;
    new->position.x = args->position.x;
    new->position.y = args->position.y;
    new->reachable = args->parent ? args->parent->reachable + 1 : 0;
    new->heuristic = new->reachable + MANHATTAN((&new->position), args->goal);

    /* Find the proper index for the new node. */
    int i;
    for (i = queue->head; i < queue->tail; ++i) {
        if (new->heuristic < queue->queue[i]->heuristic)
            break;
    }

    /* Shift all of the nodes proceeding the new node towards the tail and
    insert the new node into the queue. */
    for (int j = queue->tail; j > i; --j)
        queue->queue[j] = queue->queue[j - 1];

    queue->queue[i] = new;
    ++queue->tail;
}

/**
 * Dequeues a node from the priority queue.
 */
static struct node *dequeue(struct priority_queue *queue)
{
    if (queue->head == queue->tail) {
        fprintf(stderr, "Queue empty\n");
        return NULL;
    }

    struct node *head = queue->queue[queue->head];
    ++queue->head;
    return head;
}

/**
 * Generates the possible moves from a given node and enqueues moves to any
 * position that has not been seen already.
 */
static void enqueue_moves(struct priority_queue *queue, struct node *node, struct vec2 *apple, uint8_t seen[][GRID_SIZE])
{
    static const struct vec2 offsets[N_DIRECTIONS] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    /* Every move starting from the node has the same parent and goal. */
    struct node_args args;
    args.parent = node;
    args.goal = apple;

    /* Test each direction, adding a move to the queue if it is in bounds and
    hasn't been seen yet. */
    for (int i = 0; i < N_DIRECTIONS; ++i) {
        args.position.x = node->position.x + offsets[i].x;
        args.position.y = node->position.y + offsets[i].y;
        if (IN_BOUNDS(args.position.x, args.position.y) && !seen[args.position.x][args.position.y]) {
            enqueue(queue, &args);
            seen[args.position.x][args.position.y] = 1;
        }
    }
}

/**
 * Searches for a path from the head of the snake to the apple using the A*
 * algorithm.
 */
void search_a_star(struct snake *snake, struct vec2 *apple)
{
    /* The segments of the snake's body are the only cells seen before the
    search. */
    uint8_t seen[GRID_SIZE][GRID_SIZE] = {0};
    for (int i = 0; i < snake->length; ++i)
        seen[snake->body[i].x][snake->body[i].y] = 1;

    /* Initilize the priority queue with the snake's head. */
    struct priority_queue *queue = queue_create();
    struct node_args head_args = {NULL, snake->body[0], apple};
    enqueue(queue, &head_args);

    struct node *node;
    snake->links = 0;
    while (queue->head < queue->tail) {
        node = dequeue(queue);

        /* If the search has found the goal, step backward through the parent
        of each node from the goal until reaching the head of the snake and
        end the search. */
        if (node->position.x == apple->x && node->position.y == apple->y) {
            while (node->parent) {
                snake->chain[snake->links].x = node->position.x;
                snake->chain[snake->links].y = node->position.y;
                node = node->parent;
                ++snake->links;
            }

            queue_destroy(queue);
            return;
        }

        /* If the search hasn't found the goal, enqueue the moves from the
        highest priority node. */
        enqueue_moves(queue, node, apple, seen);
    }

    queue_destroy(queue);
}
