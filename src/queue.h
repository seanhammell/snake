#ifndef SNAKE_QUEUE_H
#define SNAKE_QUEUE_H

#include "src/constants.h"
#include "src/graph.h"

struct priority_queue {
    struct vertex *queue[N_VERTICES];
    int head;
    int tail;
};

void queue_reset(struct priority_queue *queue);
void queue_enqueue(struct priority_queue *queue, struct vertex *vertex);
struct vertex *queue_dequeue(struct priority_queue *queue);

int queue_contains(struct priority_queue *queue, int u);
void queue_decrease_key(struct priority_queue *queue, struct vertex *vertex);

#endif  /* SNAKE_QUEUE_H */
