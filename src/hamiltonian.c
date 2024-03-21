#include "src/hamiltonian.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "src/constants.h"

#define EDGE_UP(v)          (v > -1)
#define EDGE_DOWN(v)        (v < N_VERTICES)
#define EDGE_LEFT(u, v)     (((u % GRAPH_SIZE) > (v % GRAPH_SIZE)) && (v > -1))
#define EDGE_RIGHT(u, v)    ((u % GRAPH_SIZE) < (v % GRAPH_SIZE))

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

struct vertex_queue {
    struct vertex *queue[N_VERTICES];
    int head;
    int tail;
};

struct hamiltonian_step {
    int index;
    int direction;
};

static const int graph_offsets[N_DIRECTIONS] = {-GRAPH_SIZE, GRAPH_SIZE, -1, 1};
static struct hamiltonian_step hamiltonian_cycle[GRID_SIZE][GRID_SIZE];

/**
 * Resets the vertex queue.
 */
static void reset_vertex_queue(struct vertex_queue *queue)
{
    for (int i = 0; i < N_VERTICES; ++i)
        queue->queue[i] = NULL;
    queue->head = 0;
    queue->tail = 0;
}

/**
 * Enqueues a vertex to the queue.
 */
static void enqueue_vertex(struct vertex_queue *queue, struct vertex *vertex)
{
    queue->queue[queue->tail] = vertex;
    ++queue->tail;
}

/**
 * Dequeues a vertex from the queue.
 */
static struct vertex *dequeue_vertex(struct vertex_queue *queue)
{
    struct vertex *u = queue->queue[queue->head];
    ++queue->head;
    return u;
}

/**
 * Returns whether the queue contains the specified vertex.
 */
static int contains(struct vertex_queue *queue, int u)
{
    for (int i = queue->head; i < queue->tail; ++i) {
        if (queue->queue[i]->u == u)
            return 1;
    }

    return 0;
}

/**
 * Updates position of a vertex in the queue after its key is decreased by
 * Prim's algorithm.
 */
static void decrease_key(struct vertex_queue *queue, struct vertex *vertex)
{
    /* Remove the vertex from the queue. */
    int position;
    for (position = queue->head; position < queue->tail; ++position) {
        if (queue->queue[position]->u == vertex->u) {
            queue->queue[position] = NULL;
            break;
        }
    }

    /* Move all subsequent vertices up to fill the vacancy. */
    for (int i = position; i < queue->tail - 1; ++i)
        queue->queue[i] = queue->queue[i + 1];
    queue->queue[queue->tail - 1] = NULL;

    /* Find the new position for the vertex based on the updated key. */
    for (position = queue->head; position < queue->tail; ++position) {
        if (queue->queue[position] && vertex->key < queue->queue[position]->key)
            break;
    }

    /* Add the vertex back into the queue. */
    for (int j = queue->tail - 1; j > position; --j)
        queue->queue[j] = queue->queue[j - 1];
    queue->queue[position] = vertex;
}

/**
 * Adds the edge in the given direction to the vertex, if possible.
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
 * Creates an adjacency list of the snake board, scaled down by a factor of 2
 * in the x and y dimensions.
 */
static void construct_adjacency_list(struct vertex adjacency_list[N_VERTICES])
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
static void mst_prim(struct vertex adjacency_list[N_VERTICES], struct vertex *root)
{
    root->key = 0;
    struct vertex_queue queue;
    reset_vertex_queue(&queue);

    /* Add all of the vertices to the queue, starting with the root. */
    enqueue_vertex(&queue, root);
    for (int i = 0; i < N_VERTICES; ++i) {
        if (adjacency_list[i].u == root->u)
            continue;
        enqueue_vertex(&queue, &adjacency_list[i]);
    }

    /* Build a minimum spanning tree by adding the minimum-weight edge of 
    vertices in the tree to the tree until all vertices are present. */
    struct vertex *u;
    struct vertex *v;
    while (queue.head < queue.tail) {
        u = dequeue_vertex(&queue);
        for (int i = 0; i < u->degree; ++i) {
            v = &adjacency_list[u->edges[i].v];
            if (contains(&queue, v->u) && u->edges[i].weight < v->key) {
                v->parent = u;
                v->key = u->edges[i].weight;
                decrease_key(&queue, v);
            }
        }
    }
}

/**
 * Generates a Hamiltonian cycle by outlining a minimum spanning tree.
 */
void hamiltonian_generate(void)
{
    struct vertex adjacency_list[N_VERTICES];
    construct_adjacency_list(adjacency_list);
    mst_prim(adjacency_list, &adjacency_list[0]);

    /* Initialize all 2x2 grids with a clockwise cycle. */
    for (int x = 0; x < GRID_SIZE; x += 2) {
        for (int y = 0; y < GRID_SIZE; y += 2) {
            hamiltonian_cycle[x][y + 1].direction = UP;
            hamiltonian_cycle[x + 1][y].direction = DOWN;
            hamiltonian_cycle[x + 1][y + 1].direction = LEFT;
            hamiltonian_cycle[x][y].direction = RIGHT;
        }
    }

    /* Construct the full hamiltonian cycle by connecting child vertices to
    their parents. */
    for (int i = 0; i < N_VERTICES; ++i) {
        int x = (adjacency_list[i].u % GRAPH_SIZE) * 2;
        int y = (adjacency_list[i].u / GRAPH_SIZE) * 2;
        if (adjacency_list[i].parent) {
            int offset = adjacency_list[i].parent->u - adjacency_list[i].u;
            if (offset == graph_offsets[UP]) {
                hamiltonian_cycle[x][y].direction = UP;
                hamiltonian_cycle[x + 1][y - 1].direction = DOWN;
            } else if (offset == graph_offsets[DOWN]) {
                hamiltonian_cycle[x + 1][y + 1].direction = DOWN;
                hamiltonian_cycle[x][y + 2].direction = UP;
            } else if (offset == graph_offsets[LEFT]) {
                hamiltonian_cycle[x][y + 1].direction = LEFT;
                hamiltonian_cycle[x - 1][y].direction = RIGHT;
            } else if (offset == graph_offsets[RIGHT]) {
                hamiltonian_cycle[x + 1][y].direction = RIGHT;
                hamiltonian_cycle[x + 2][y + 1].direction = LEFT;
            }
        }
    }

    /* Step through the tree to set the indices for each position. */
    struct vec2 step = {0, 0};
    int direction;
    for (int i = 0; i < N_CELLS; ++i) {
        hamiltonian_cycle[step.x][step.y].index = i;
        direction = hamiltonian_cycle[step.x][step.y].direction;
        step.x += offsets[direction].x;
        step.y += offsets[direction].y;
    }

    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x)
            fprintf(stderr, "%4d ", hamiltonian_cycle[x][y].index);

        fprintf(stderr, "\n");
    }

    fprintf(stderr, "\n");
}

/**
 * Returns the index of the position in the hamiltonian cycle.
 */
int hamiltonian_index(struct vec2 *position)
{
    return hamiltonian_cycle[position->x][position->y].index;
}
