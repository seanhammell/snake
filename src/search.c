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
#include <stdio.h>
#include <stdlib.h>

#include "src/game.h"
#include "src/search.h"

#define MAX_QUEUE_SIZE  (GRID_SIZE * GRID_SIZE)

#define MANHATTAN(a, b) (abs(a->x - b->x) + abs(a->y - b->y))

struct node {
    struct node *parent;
    struct vec2 position;
    int reachable;
    int heuristic;
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
static void enqueue(struct priority_queue *queue, struct node *parent, struct vec2 *position, struct vec2 *goal)
{
    if (queue->tail == MAX_QUEUE_SIZE) {
        fprintf(stderr, "Queue full\n");
        return;
    }

    struct node *new = queue->queue[queue->tail];
    new->parent = parent;
    new->position.x = position->x;
    new->position.y = position->y;
    new->reachable = parent ? parent->reachable + 1 : 0;
    new->heuristic = new->reachable + MANHATTAN((&new->position), goal);

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

static void enqueue_moves(struct node *node, struct vec2 *apple, struct priority_queue *queue, int occupancy[][GRID_SIZE])
{
    static const struct vec2 offsets[N_DIRECTIONS] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    for (int i = 0; i < N_DIRECTIONS; ++i) {
        struct vec2 pos;
        pos.x = node->position.x + offsets[i].x;
        pos.y = node->position.y + offsets[i].y;
        if (pos.x > -1 && pos.y > -1 && pos.x < GRID_SIZE && pos.y < GRID_SIZE) {
            if (!occupancy[pos.x][pos.y]) {
                int i;
                for (i = queue->head; i < queue->tail; ++i) {
                    if (queue->queue[i]->position.x == pos.x && queue->queue[i]->position.y == pos.y)
                        break;
                }

                if (i == queue->tail)
                    enqueue(queue, node, &pos, apple);
            }
        }
    }
}

void search_driver(struct snake *snake, struct vec2 *apple)
{
    int occupancy[GRID_SIZE][GRID_SIZE] = {0};
    for (int i = 0; i < snake->length; ++i)
        occupancy[snake->body[i].x][snake->body[i].y] = 1;

    struct priority_queue *queue = queue_create();
    enqueue(queue, NULL, &snake->body[0], apple);

    struct node *node;
    snake->links = 0;
    while (queue->head < queue->tail) {
        node = dequeue(queue);
        occupancy[node->position.x][node->position.y] = 1;

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

        enqueue_moves(node, apple, queue, occupancy);
    }

    queue_destroy(queue);
}
