#include "src/search.h"

#include "src/snake.h"

#define LEFT_COLUMN(x, y)   (x == 0 && y < GRID_SIZE - 1)
#define ODD_ROW(x, y)       (y % 2)
#define EVEN_ROW(x, y)      (!ODD_ROW(x, y))
#define END_OF_ROW(x, y)    ((x == 1 && EVEN_ROW(x, y) && y > 0) || (x == GRID_SIZE - 1 && ODD_ROW(x, y)))

void search_hamiltonian_cycle(struct snake *snake)
{
    int x = snake->body[0].x;
    int y = snake->body[0].y;
    if (LEFT_COLUMN(x, y))
        snake->direction = DOWN;
    else if (END_OF_ROW(x, y))
        snake->direction = UP;
    else if (EVEN_ROW(x, y))
        snake->direction = LEFT;
    else if (ODD_ROW(x, y))
        snake->direction = RIGHT;
}
