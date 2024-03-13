#ifndef SNAKE_GRAPH_H
#define SNAKE_GRAPH_H

#include "src/constants.h"

struct edge {
    int u;
    int v;
    int weight;
};

struct vertex {
    int u;
    int degree;
    struct edge edges[N_DIRECTIONS];

    int key;
    struct vertex *parent;
};

void graph_construct_adjacency_list(struct vertex adjacency_list[N_VERTICES]);
void graph_mst_prim(struct vertex adjacency_list[N_VERTICES], struct vertex *root);

#endif  /* SNAKE_GRAPH_H */
