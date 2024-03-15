#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDL2/SDL.h"

#include "src/game.h"

#define N_TRIALS    1

int main(void)
{
    srand(time(NULL));

    struct graphics *graphics = graphics_create();
    if (graphics == NULL)
        return 0;

    int steps = 0;
    int length = 0;
    int completed = 0;
    int total = 0;

    fprintf(stderr, "\n");
    for (int i = 0; i < N_TRIALS; ++i) {
        ++total;
        if (game_loop(graphics, &steps, &length))
            ++completed;

        fprintf(stderr,
                "\rCompleted:%7.2f%%, Average Steps:%5d, Average Length:%6.2f",
                (completed / (float) total) * 100,
                steps / completed,
                length / (float) total);
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
