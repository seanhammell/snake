#include "src/search.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "src/constants.h"
#include "src/graph.h"
#include "src/snake.h"

#define IN_BOUNDS(s)            (s.x > -1 && s.y > -1 && s.x < GRID_SIZE && s.y < GRID_SIZE)
#define CHOMP(s, a)             (s->x == a->x && s->y == a->y)
#define MAX_CUT(snake, s, a)    (((N_CELLS - snake->length) * 0.4) - CHOMP(s, a))

struct path_step {
    int index;
    int direction;
};

static struct path_step hamiltonian_cycle[GRID_SIZE][GRID_SIZE];

/**
 * Determines the distance from the step to the apple along the Hamiltonian
 * cycle.
 */
int path_distance(struct vec2 *step, struct vec2 *apple)
{
    int step_index = hamiltonian_cycle[step->x][step->y].index;
    int apple_index = hamiltonian_cycle[apple->x][apple->y].index;
    if (apple_index < step_index)
        apple_index += N_CELLS;

    return apple_index - step_index;
}

/**
 * Ensures the step does not cut too much of the Hamiltonian cycle or overtake
 * the snake's tail.
 */
int safe_path(struct snake *snake, struct vec2 *step, struct vec2 *apple)
{
    int step_index = hamiltonian_cycle[step->x][step->y].index;
    int head_index = hamiltonian_cycle[snake->body[0].x][snake->body[0].y].index;
    int tail_index = hamiltonian_cycle[snake->body[snake->length - 1].x][snake->body[snake->length - 1].y].index;
    int cut = step_index < head_index ? step_index + N_CELLS - head_index : step_index - head_index;

    if (cut > MAX_CUT(snake, step, apple))
        return 0;

    for (int i = step_index; i != tail_index; i = (i + 1) % N_CELLS) {
        for (int j = 1; j < snake->length; ++j) {
            if (i == hamiltonian_cycle[snake->body[j].x][snake->body[j].y].index)
                return 0;
        }
    }

    return 1;
}

/**
 * Generates a Hamiltonian cycle by outlining a minimum spanning tree.
 */
void search_generate_hamiltonian_cycle(void)
{
    struct vertex adjacency_list[N_VERTICES];
    graph_construct_adjacency_list(adjacency_list);
    graph_mst_prim(adjacency_list, &adjacency_list[0]);

    /* Initialize each vertex with a clockwise cycle. */
    for (int x = 0; x < GRID_SIZE; x += 2) {
        for (int y = 0; y < GRID_SIZE; y += 2) {
            hamiltonian_cycle[x][y + 1].direction = UP;
            hamiltonian_cycle[x + 1][y].direction = DOWN;
            hamiltonian_cycle[x + 1][y + 1].direction = LEFT;
            hamiltonian_cycle[x][y].direction = RIGHT;
        }
    }

    /* Connect parent and child vertices of the minimum spanning tree. */
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

    /* Starting in the top left corner, number off the steps in the Hamiltonian
    cycle. */
    int x = 0;
    int y = 0;
    for (int counter = 0; counter < N_CELLS; ++counter) {
        hamiltonian_cycle[x][y].index = counter;
        switch (hamiltonian_cycle[x][y].direction) {
        case UP:
            y -= 1;
            break;
        case DOWN:
            y += 1;
            break;
        case LEFT:
            x -= 1;
            break;
        case RIGHT:
            x += 1;
            break;
        default:
            break;
        }
    }
}

/**
 * Updates the snake's direction based on the pathfinding algorithm.
 */
void search_pathfinder(struct snake *snake, struct vec2 *apple)
{
    struct vec2 *head = &snake->body[0];
    snake->direction = hamiltonian_cycle[head->x][head->y].direction;
    if (snake->length > (0.5 * N_CELLS))
        return;

    uint8_t occupied[GRID_SIZE][GRID_SIZE] = {0};
    for (int i = 0; i < snake->length; ++i)
        occupied[snake->body[i].x][snake->body[i].y] = 1;

    struct vec2 step;
    int shortest = N_CELLS;
    for (int d = 0; d < N_DIRECTIONS; ++d) {
        step.x = head->x + step_offsets[d].x;
        step.y = head->y + step_offsets[d].y;
        int distance = path_distance(&step, apple);
        if (IN_BOUNDS(step) && !occupied[step.x][step.y] && distance < shortest && safe_path(snake, &step, apple)) {
            shortest = distance;
            snake->direction = d;
        }
    }
}
