#ifndef SNAKE_SEARCH_H
#define SNAKE_SEARCH_H

#include "src/snake.h"

void search_generate_hamiltonian_cycle(void);
void search_pathfinder(struct snake *snake, struct vec2 *apple);

#endif  /* SNAKE_SEARCH_H */
