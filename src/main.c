#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDL2/SDL.h"

#include "src/constants.h"
#include "src/game.h"

#define N_TRIALS    1

struct search_info search_info;

int main(void)
{
    srand(time(NULL));

    struct graphics *graphics = graphics_create();
    if (graphics == NULL)
        return 0;

    search_info.steps = 0;
    search_info.length = 0;
    search_info.nodes = 0;
    search_info.completed = 0;
    search_info.total = 0;

    fprintf(stderr, "\n");
    for (int i = 0; i < N_TRIALS; ++i) {
        ++search_info.total;
        if (game_loop(graphics))
            ++search_info.completed;

        fprintf(stderr,
                "\r%3d Trial(s), Completed: %d/%d, Average Length:%7.2f, Average Steps:%5d, Average Nodes: %8d",
                i + 1,
                search_info.completed,
                search_info.total,
                search_info.length / (float) search_info.total,
                search_info.steps / search_info.completed,
                search_info.nodes / search_info.completed);
    }

    fprintf(stderr, "\n\n");

    SDL_Event e;
    for (;;) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                graphics_destroy(graphics);
                return 0;
            }
        }
    }

    return 0;
}
