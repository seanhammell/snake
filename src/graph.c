#include "src/graph.h"

#include <limits.h>
#include <stdlib.h>
#include <time.h>

#include "src/queue.h"

#define EDGE_UP(v)          (v > -1)
#define EDGE_DOWN(v)        (v < N_VERTICES)
#define EDGE_LEFT(u, v)     (((u % GRAPH_SIZE) > (v % GRAPH_SIZE)) && (v > -1))
#define EDGE_RIGHT(u, v)    ((u % GRAPH_SIZE) < (v % GRAPH_SIZE))

/**
 * Adds an edge in the given direction to the vertex, if possible.
 */
static void add_edge(struct vertex *vertex, struct edge *edge, int direction)
{
    edge->u = vertex->u;
    edge->v = vertex->u + graph_offsets[direction];
    edge->weight = rand() % N_VERTICES;

    if ((direction == UP && EDGE_UP(edge->v)) ||
        (direction == DOWN && EDGE_DOWN(edge->v)) ||
        (direction == LEFT && EDGE_LEFT(edge->u, edge->v)) ||
        (direction == RIGHT && EDGE_RIGHT(edge->u, edge->v)))
        ++vertex->degree;
}

/**
 * Creates a adjacency list graph representation of the snake board.
 */
void graph_construct_adjacency_list(struct vertex adjacency_list[N_VERTICES])
{
    struct vertex *vertex;
    for (int i = 0; i < N_VERTICES; ++i) {
        vertex = &adjacency_list[i];
        vertex->u = i;
        vertex->degree = 0;
        for (int d = UP; d < N_DIRECTIONS; ++d)
            add_edge(vertex, &vertex->edges[vertex->degree], d);

        vertex->key = INT_MAX;
        vertex->parent = NULL;
    }
}

/**
 * Builds a minimum spanning tree using Prim's algorithm.
 */
void graph_mst_prim(struct vertex adjacency_list[N_VERTICES], struct vertex *root)
{
    root->key = 0;
    struct priority_queue queue;
    queue_reset(&queue);

    queue_enqueue(&queue, root);
    for (int i = 0; i < N_VERTICES; ++i) {
        if (adjacency_list[i].u == root->u)
            continue;
        queue_enqueue(&queue, &adjacency_list[i]);
    }

    struct vertex *u;
    struct vertex *v;
    while (queue.head < queue.tail) {
        u = queue_dequeue(&queue);
        for (int i = 0; i < u->degree; ++i) {
            v = &adjacency_list[u->edges[i].v];
            if (queue_contains(&queue, v->u) && u->edges[i].weight < v->key) {
                v->parent = u;
                v->key = u->edges[i].weight;
                queue_decrease_key(&queue, v);
            }
        }
    }
}
