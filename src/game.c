#include "src/game.h"

#include <stdint.h>

#include "SDL2/SDL.h"

#include "src/constants.h"
#include "src/graphics.h"
#include "src/search.h"
#include "src/snake.h"

/**
 * Updates the snake's direction based on user input.
 */
static void input(SDL_Event *e, struct snake *snake)
{
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
        case SDLK_UP:
            if (snake->length < 2 || snake->body[0].y - 1 != snake->body[1].y)
                snake->direction = UP;
            break;
        case SDLK_DOWN:
            if (snake->length < 2 || snake->body[0].y + 1 != snake->body[1].y)
                snake->direction = DOWN;
            break;
        case SDLK_LEFT:
            if (snake->length < 2 || snake->body[0].x - 1 != snake->body[1].x)
                snake->direction = LEFT;
            break;
        case SDLK_RIGHT:
            if (snake->length < 2 || snake->body[0].x + 1 != snake->body[1].x)
                snake->direction = RIGHT;
            break;
        default:
            break;
        }
    }
}

/**
 * Moves the snake on a given interval and checks if the snake is eating the
 * apple or biting its tail.
 */
static int update(uint64_t dt, struct snake *snake)
{
    static const uint64_t interval = 20;
    static uint64_t elapsed = 0;

    elapsed += dt;
    while (elapsed > interval) {
        ++search_info.steps;
        elapsed -= interval;
        search_pathfinder(snake);
        snake_move(snake);

        if (snake_biting_tail(snake) || FULL_SNAKE(snake)) {
            elapsed = 0;
            snake->direction = STOP;
            return FULL_SNAKE(snake);
        }
    }

    return -1;
}

/**
 * Renders the snake and apple to the screen.
 */
static void render(struct graphics *graphics, struct snake *snake)
{
    SDL_Rect rect = {0, 0, 14, 14};
    rect.x = snake->apple.x * TILE_SIZE;
    rect.y = snake->apple.y * TILE_SIZE;
    SDL_SetRenderDrawColor(graphics->renderer, 0xAC, 0x38, 0x38, 0xFF);
    SDL_RenderFillRect(graphics->renderer, &rect);

    SDL_SetRenderDrawColor(graphics->renderer, 0x40, 0x98, 0x5E, 0xFF);
    for (int i = 0; i < snake->length; ++i) {
        rect.x = snake->body[i].x * TILE_SIZE;
        rect.y = snake->body[i].y * TILE_SIZE;
        SDL_RenderFillRect(graphics->renderer, &rect);

        if (i > 0) {
            rect.x += (snake->body[i - 1].x - snake->body[i].x) * 2;
            rect.y += (snake->body[i - 1].y - snake->body[i].y) * 2;
            SDL_RenderFillRect(graphics->renderer, &rect);
        }
    }
}

/**
 * Controls the main game loop.
 */
int game_loop(struct graphics *graphics)
{
    static uint64_t timer = 0;

    struct snake *snake = snake_create();

    SDL_Event e;
    int steps_copy = search_info.steps;
    int nodes_copy = search_info.nodes;
    for (;;) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                snake_destroy(snake);
                return 0;
            }

            input(&e, snake);
        }

        uint64_t now = SDL_GetTicks64();
        int status = update(now - timer, snake);
        timer = now;

        SDL_SetRenderDrawColor(graphics->renderer, 0x11, 0x11, 0x11, 0xFF);
        SDL_RenderClear(graphics->renderer);
        render(graphics, snake);
        SDL_RenderPresent(graphics->renderer);

        if (status > -1) {
            if (!status) {
                search_info.steps = steps_copy;
                search_info.nodes = nodes_copy;
            }

            search_info.length += snake->length + status;
            return status;
        }
    }
}
