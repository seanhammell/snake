#include "src/queue.h"

#include <stdlib.h>

#include "src/constants.h"

/**
 * Resets the priority queue.
 */
void queue_reset(struct priority_queue *queue)
{
    for (int i = 0; i < N_VERTICES; ++i)
        queue->queue[i] = NULL;
    queue->head = 0;
    queue->tail = 0;
}

/**
 * Adds a vertex to the priority queue. All elements have the same value when
 * enqueued, so sorting is handled by queue_decrease_key when graph_mst_prim
 * updates an element.
 */
void queue_enqueue(struct priority_queue *queue, struct vertex *vertex)
{
    queue->queue[queue->tail] = vertex;
    ++queue->tail;
}

/**
 * Removes a vertex from the priority queue.
 */
struct vertex *queue_dequeue(struct priority_queue *queue)
{
    struct vertex *u = queue->queue[queue->head];
    ++queue->head;
    return u;
}

/**
 * Returns if the priority queue contains the specified element.
 */
int queue_contains(struct priority_queue *queue, int u)
{
    for (int i = queue->head; i < queue->tail; ++i) {
        if (queue->queue[i]->u == u)
            return 1;
    }

    return 0;
}

/**
 * Updates the priority queue after an element's key is decreased.
 */
void queue_decrease_key(struct priority_queue *queue, struct vertex *vertex)
{
    int position;
    for (position = queue->head; position < queue->tail; ++position) {
        if (queue->queue[position]->u == vertex->u) {
            queue->queue[position] = NULL;
            break;
        }
    }

    for (int i = position; i < queue->tail - 1; ++i)
        queue->queue[i] = queue->queue[i + 1];
    queue->queue[queue->tail - 1] = NULL;

    for (position = queue->head; position < queue->tail; ++position) {
        if (queue->queue[position] && vertex->key < queue->queue[position]->key)
            break;
    }

    for (int j = queue->tail - 1; j > position; --j)
        queue->queue[j] = queue->queue[j - 1];
    queue->queue[position] = vertex;
}
