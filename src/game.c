#include "src/game.h"

#include <stdint.h>
#include <stdlib.h>

#include "SDL2/SDL.h"

#include "src/graphics.h"
#include "src/snake.h"

#define ENTITY_SIZE 14
#define TILE_SIZE   16

void random_apple(struct vec2 *apple, struct snake *snake)
{
    int occupied;
    do {
        apple->x = rand() % GRID_SIZE;
        apple->y = rand() % GRID_SIZE;
        occupied = 0;
        for (int i = 0; i < snake->length; ++i) {
            if (apple->x == snake->body[i].x && apple->y == snake->body[i].y) {
                occupied = 1;
                break;
            }
        }
    } while (occupied);
}

static void input(SDL_Event *e, struct snake *snake)
{
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
        case SDLK_UP:
            if (snake->body[0].y - 1 != snake->body[1].y)
                snake->direction = UP;
            break;
        case SDLK_DOWN:
            if (snake->body[0].y + 1 != snake->body[1].y)
                snake->direction = DOWN;
            break;
        case SDLK_LEFT:
            if (snake->body[0].x - 1 != snake->body[1].x)
                snake->direction = LEFT;
            break;
        case SDLK_RIGHT:
            if (snake->body[0].x + 1 != snake->body[1].x)
                snake->direction = RIGHT;
            break;
        default:
            break;
        }
    }
}

static void update(uint64_t dt, struct snake *snake, struct vec2 *apple)
{
    static const uint64_t interval = 80;
    static uint64_t elapsed = 0;
    elapsed += dt;
    while (elapsed > interval) {
        elapsed -= interval;
        snake_move(snake);

        if (EATING_APPLE(snake, apple)) {
            snake_grow(snake);
            random_apple(apple, snake);
        }
    }
}

static void render(struct graphics *graphics, struct snake *snake, struct vec2 *apple)
{
    SDL_Rect rect = {0, 0, ENTITY_SIZE, ENTITY_SIZE};
    rect.x = apple->x * TILE_SIZE;
    rect.y = apple->y * TILE_SIZE;
    SDL_SetRenderDrawColor(graphics->renderer, 0xAC, 0x38, 0x38, 0xFF);
    SDL_RenderFillRect(graphics->renderer, &rect);

    for (int i = 0; i < snake->length; ++i) {
        rect.x = snake->body[i].x * TILE_SIZE;
        rect.y = snake->body[i].y * TILE_SIZE;
        SDL_SetRenderDrawColor(graphics->renderer, 0x40, 0x98, 0x5E, 0xFF);
        SDL_RenderFillRect(graphics->renderer, &rect);
    }
}

void game_loop(void)
{
    struct graphics *graphics = graphics_create();
    if (graphics == NULL)
        return;

    struct snake *snake = snake_create();
    struct vec2 apple;
    random_apple(&apple, snake);

    SDL_Event e;
    uint64_t timer = 0;
    for (;;) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                snake_destroy(snake);
                graphics_destroy(graphics);
                return;
            }

            input(&e, snake);
        }

        uint64_t now = SDL_GetTicks64();
        update(now - timer, snake, &apple);
        timer = now;

        SDL_SetRenderDrawColor(graphics->renderer, 0x11, 0x11, 0x11, 0xFF);
        SDL_RenderClear(graphics->renderer);
        render(graphics, snake, &apple);
        SDL_RenderPresent(graphics->renderer);
    }
}
