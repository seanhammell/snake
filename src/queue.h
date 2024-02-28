#ifndef SNAKE_QUEUE_H
#define SNAKE_QUEUE_H

#include "src/snake.h"

#define MAX_QUEUE_SIZE  (GRID_SIZE * GRID_SIZE)

struct node {
  struct vec2 position;
  int reachable;
  int heuristic;
};

struct node_args {
  struct vec2 position;
  int reachable;
  struct vec2 *goal;
};

struct priority_queue {
  struct node *queue[MAX_QUEUE_SIZE];
  int head;
  int tail;
};

struct priority_queue *queue_create(void);
void queue_destroy(struct priority_queue *queue);

void queue_enqueue(struct priority_queue *queue, struct node_args *args);
struct node *queue_dequeue(struct priority_queue *queue);

#endif  /* SNAKE_QUEUE_H */
