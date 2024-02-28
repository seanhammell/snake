#include "src/queue.h"

#include <stdio.h>
#include <stdlib.h>

#define MANHATTAN(a, b) (abs(a->x - b->x) + abs(a->y - b->y))

struct priority_queue *queue_create(void)
{
    struct priority_queue *queue = malloc(sizeof(struct priority_queue));
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i) {
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

void queue_destroy(struct priority_queue *queue)
{
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i)
        free(queue->queue[i]);
    free(queue);
}

void queue_enqueue(struct priority_queue *queue, struct node_args *args)
{
    if (queue->tail == MAX_QUEUE_SIZE) {
        fprintf(stderr, "Queue full\n");
        return;
    }

    struct node *new = queue->queue[queue->tail];
    new->position.x = args->position.x;
    new->position.y = args->position.y;
    new->reachable = args->reachable;
    new->heuristic = new->reachable + MANHATTAN((&new->position), args->goal);

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

struct node *queue_dequeue(struct priority_queue *queue)
{
    if (queue->head == queue->tail) {
        fprintf(stderr, "Queue empty\n");
        return NULL;
    }

    struct node *head = queue->queue[queue->head];
    ++queue->head;
    return head;
}
