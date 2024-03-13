#include "src/search.h"

#include "src/constants.h"
#include "src/graph.h"
#include "src/snake.h"

static int hamiltonian[GRID_SIZE][GRID_SIZE];

/**
 * Generates a Hamiltonian cycle by outlining a minimum spanning tree.
 */
void search_generate_hamiltonian(void)
{
    struct vertex adjacency_list[N_VERTICES];
    graph_construct_adjacency_list(adjacency_list);
    graph_mst_prim(adjacency_list, &adjacency_list[0]);

    for (int x = 0; x < GRID_SIZE; x += 2) {
        for (int y = 0; y < GRID_SIZE; y += 2) {
            hamiltonian[x][y + 1] = UP;
            hamiltonian[x + 1][y] = DOWN;
            hamiltonian[x + 1][y + 1] = LEFT;
            hamiltonian[x][y] = RIGHT;
        }
    }

    for (int i = 0; i < N_VERTICES; ++i) {
        int x = (adjacency_list[i].u % GRAPH_SIZE) * 2;
        int y = (adjacency_list[i].u / GRAPH_SIZE) * 2;
        if (adjacency_list[i].parent) {
            int offset = adjacency_list[i].parent->u - adjacency_list[i].u;
            if (offset == graph_offsets[UP]) {
                hamiltonian[x][y] = UP;
                hamiltonian[x + 1][y - 1] = DOWN;
            } else if (offset == graph_offsets[DOWN]) {
                hamiltonian[x + 1][y + 1] = DOWN;
                hamiltonian[x][y + 2] = UP;
            } else if (offset == graph_offsets[LEFT]) {
                hamiltonian[x][y + 1] = LEFT;
                hamiltonian[x - 1][y] = RIGHT;
            } else if (offset == graph_offsets[RIGHT]) {
                hamiltonian[x + 1][y] = RIGHT;
                hamiltonian[x + 2][y + 1] = LEFT;
            }
        }
    }
}

/**
 * Updates the snake's direction based on the pathfinding algorithm.
 */
void search_pathfinder(struct snake *snake)
{
    snake->direction = hamiltonian[snake->body[0].x][snake->body[0].y];
}
