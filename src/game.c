#include <stdint.h>
#include <stdlib.h>

#include "SDL2/SDL.h"

#include "src/game.h"
#include "src/graphics.h"

#define ENTITY_SIZE 14
#define TILE_SIZE 16
#define GRID_SIZE 32

enum directions {
    STOP,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct segment {
    int x;
    int y;
    int dx;
    int dy;
};

struct snake {
    struct segment body[GRID_SIZE * GRID_SIZE];
    int length;
    int direction;
    int offset;
};

struct apple {
    int x;
    int y;
};

static void reset_snake(struct snake *snake, int (*occupancy)[GRID_SIZE])
{
    snake->body[0].x = (GRID_SIZE / 2) - 1;
    snake->body[0].y = (GRID_SIZE / 2) - 1;
    snake->body[0].dx = 0;
    snake->body[0].dy = 0;
    snake->length = 1;
    snake->direction = STOP;

    memset(occupancy, 0, GRID_SIZE * GRID_SIZE);
    occupancy[snake->body[0].x][snake->body[0].y] = 1;
}

static void random_apple(struct apple *apple, int (*occupancy)[GRID_SIZE])
{
    do {
        apple->x = rand() % GRID_SIZE;
        apple->y = rand() % GRID_SIZE;
    } while (occupancy[apple->x][apple->y]);
}

static void input(SDL_Event *e, struct snake *snake)
{
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
        case SDLK_UP:
            if (snake->body[0].dy != 1)
                snake->direction = UP;
            break;
        case SDLK_DOWN:
            if (snake->body[0].dy != -1)
                snake->direction = DOWN;
            break;
        case SDLK_LEFT:
            if (snake->body[0].dx != 1)
                snake->direction = LEFT;
            break;
        case SDLK_RIGHT:
            if (snake->body[0].dx != -1)
                snake->direction = RIGHT;
            break;
        default:
            break;
        }
    }
}

static void update(uint64_t dt, struct snake *snake, struct apple *apple, int (*occupancy)[GRID_SIZE])
{
    static const uint64_t interval = 80;
    static uint64_t elapsed = 0;
    elapsed += dt;
    snake->offset = elapsed / (interval / TILE_SIZE);
    while (elapsed > interval) {
        elapsed -= interval;
        snake->offset = 0;

        occupancy[snake->body[snake->length - 1].x][snake->body[snake->length - 1].y] = 0;
        for (int i = snake->length - 1; i > 0; --i)
            snake->body[i] = snake->body[i - 1];

        struct segment *head = &snake->body[0];
        head->x += head->dx;
        head->y += head->dy;
        if (head->x < 0 || head->y < 0 || head->x >= GRID_SIZE || head->y >= GRID_SIZE) {
            reset_snake(snake, occupancy);
            random_apple(apple, occupancy);
            return;
        }

        occupancy[head->x][head->y] = 1;
        switch (snake->direction) {
        case UP:
            head->dx = 0;
            head->dy = -1;
            break;
        case DOWN:
            head->dx = 0;
            head->dy = 1;
            break;
        case LEFT:
            head->dx = -1;
            head->dy = 0;
            break;
        case RIGHT:
            head->dx = 1;
            head->dy = 0;
            break;
        }

        if (head->x == apple->x && head->y == apple->y) {
            random_apple(apple, occupancy);
            ++snake->length;
            if (snake->length == GRID_SIZE * GRID_SIZE)
                return;

            snake->body[snake->length - 1].x = snake->body[snake->length - 2].x;
            snake->body[snake->length - 1].y = snake->body[snake->length - 2].y;
        }
    }
}

static void render(struct graphics *graphics, struct snake *snake, struct apple *apple)
{
    SDL_Rect apple_rect = {0, 0, ENTITY_SIZE, ENTITY_SIZE};
    apple_rect.x = apple->x * TILE_SIZE;
    apple_rect.y = apple->y * TILE_SIZE;
    SDL_SetRenderDrawColor(graphics->renderer, 0xAC, 0x38, 0x38, 0xFF);
    SDL_RenderFillRect(graphics->renderer, &apple_rect);
    apple_rect.x += snake->body[0].dx;

    SDL_Rect segment = {0, 0, ENTITY_SIZE, ENTITY_SIZE};
    for (int i = 0; i < snake->length; ++i) {
        segment.x = (snake->body[i].x * TILE_SIZE); /* + (snake->offset * snake->body[i].dx); */
        segment.y = (snake->body[i].y * TILE_SIZE); /* + (snake->offset * snake->body[i].dy); */
        SDL_SetRenderDrawColor(graphics->renderer, 0x40, 0x98, 0x5E, 0xFF);
        SDL_RenderFillRect(graphics->renderer, &segment);
    }
}

void game_loop(void)
{
    struct graphics *graphics = graphics_create();
    if (graphics == NULL)
        return;

    int occupancy[GRID_SIZE][GRID_SIZE] = {0};

    struct snake snake;
    reset_snake(&snake, occupancy);

    struct apple apple;
    random_apple(&apple, occupancy);

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
        update(now - timer, &snake, &apple, occupancy);
        timer = now;

        SDL_SetRenderDrawColor(graphics->renderer, 0x11, 0x11, 0x11, 0xFF);
        SDL_RenderClear(graphics->renderer);
        render(graphics, &snake, &apple);
        SDL_RenderPresent(graphics->renderer);
    }
}
