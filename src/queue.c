#include "src/queue.h"

#include <stdio.h>
#include <stdlib.h>

#include "src/constants.h"

/**
 * Creates a new queue.
 */
struct priority_queue *queue_create(void)
{
    struct priority_queue *queue = malloc(sizeof(struct priority_queue));
    for (int i = 0; i < N_CELLS; ++i) {
        queue->queue[i] = malloc(sizeof(struct node));
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
 * Destroys the queue.
 */
void queue_destroy(struct priority_queue *queue)
{
    for (int i = 0; i < N_CELLS; ++i)
        free(queue->queue[i]);
    free(queue);
}

/**
 * Enqueues a node based on the A* function.
 */
void queue_enqueue(struct priority_queue *queue, struct node_args *args)
{
    if (queue->tail == N_CELLS) {
        fprintf(stderr, "Full Queue\n");
        return;
    }

    struct node *new = queue->queue[queue->tail];
    new->position.x = args->position.x;
    new->position.y = args->position.y;
    new->reachable = args->reachable;
    new->heuristic = new->reachable + MANHATTAN(new->position, args->goal);

    int i;
    for (i = queue->head; i < queue->tail; ++i) {
        if (new->heuristic < queue->queue[i]->heuristic)
            break;
    }

    for (int j = queue->tail; j > i; --j)
        queue->queue[j] = queue->queue[j - 1];

    queue->queue[i] = new;
    ++queue->tail;
}

/**
 * Dequeues the node at the head of the queue.
 */
struct node *queue_dequeue(struct priority_queue *queue)
{
    if (queue->head == queue->tail) {
        fprintf(stderr, "Empty Queue\n");
        return NULL;
    }

    struct node *head = queue->queue[queue->head];
    ++queue->head;
    return head;
}
