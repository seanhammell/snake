#include <stdint.h>

#include "SDL2/SDL.h"

#include "src/game.h"
#include "src/graphics.h"

#define TILE_SIZE 16
#define GRID_SIZE 32

#define ENTITY_SIZE 14

enum directions {
    STOP,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct snake_segment {
    int x;
    int y;
    int dx;
    int dy;
};

struct snake {
    struct snake_segment body[GRID_SIZE * GRID_SIZE];
    int length;
    int direction;
    int offset;
};

static void init_snake(struct snake *snake)
{
    snake->body[0].x = GRID_SIZE / 2;
    snake->body[0].y = GRID_SIZE / 2;
    snake->body[0].dx = 0;
    snake->body[0].dy = 0;
    snake->length = 1;
    snake->direction = STOP;
}

static void random_apple(SDL_Rect *apple)
{
    apple->x = 0;
    apple->y = 0;
}

static void input(SDL_Event *e, struct snake *snake)
{
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
        case SDLK_UP:
            if (snake->direction != DOWN)
                snake->direction = UP;
            break;
        case SDLK_DOWN:
            if (snake->direction != UP)
                snake->direction = DOWN;
            break;
        case SDLK_LEFT:
            if (snake->direction != RIGHT)
                snake->direction = LEFT;
            break;
        case SDLK_RIGHT:
            if (snake->direction != LEFT)
                snake->direction = RIGHT;
            break;
        default:
            break;
        }
    }
}

static void update(uint64_t dt, struct snake *snake)
{
    static const uint64_t interval = 80;
    static uint64_t elapsed = 0;
    elapsed += dt;
    snake->offset = elapsed / (interval / TILE_SIZE);
    while (elapsed > interval) {
        elapsed -= interval;
        snake->offset = 0;
        for (int i = snake->length - 1; i > 0; --i)
            snake->body[i] = snake->body[i - 1];

        snake->body[0].x += snake->body[0].dx;
        snake->body[0].y += snake->body[0].dy;
        switch (snake->direction) {
        case UP:
            snake->body[0].dx = 0;
            snake->body[0].dy = -1;
            break;
        case DOWN:
            snake->body[0].dx = 0;
            snake->body[0].dy = 1;
            break;
        case LEFT:
            snake->body[0].dx = -1;
            snake->body[0].dy = 0;
            break;
        case RIGHT:
            snake->body[0].dx = 1;
            snake->body[0].dy = 0;
            break;
        }
    }
}

static void render(struct graphics *graphics, struct snake *snake, SDL_Rect *apple)
{
    SDL_SetRenderDrawColor(graphics->renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(graphics->renderer, apple);

    SDL_Rect segment = {0, 0, ENTITY_SIZE, ENTITY_SIZE};
    for (int i = 0; i < snake->length; ++i) {
        segment.x = (snake->body[i].x * TILE_SIZE) + (snake->offset * snake->body[i].dx);
        segment.y = (snake->body[i].y * TILE_SIZE) + (snake->offset * snake->body[i].dy);
        SDL_SetRenderDrawColor(graphics->renderer, 0x40, 0x98, 0x5E, 0xFF);
        SDL_RenderFillRect(graphics->renderer, &segment);
    }
}

void game_loop(void)
{
    struct graphics *graphics = graphics_create();
    if (graphics == NULL)
        return;

    struct snake snake;
    init_snake(&snake);

    SDL_Rect apple;
    random_apple(&apple);
    apple.w = ENTITY_SIZE;
    apple.h = ENTITY_SIZE;

    SDL_Event e;
    uint64_t timer = 0;
    for (;;) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                graphics_destroy(graphics);
                return;
            }

            input(&e, &snake);
        }

        uint64_t now = SDL_GetTicks64();
        update(now - timer, &snake);
        timer = now;

        SDL_SetRenderDrawColor(graphics->renderer, 0x11, 0x11, 0x11, 0xFF);
        SDL_RenderClear(graphics->renderer);
        render(graphics, &snake, &apple);
        SDL_RenderPresent(graphics->renderer);
    }
}
