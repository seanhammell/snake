#include <stdint.h>
#include <stdlib.h>

#include "SDL2/SDL.h"

#include "src/game.h"
#include "src/graphics.h"

#define ENTITY_SIZE 14
#define TILE_SIZE   16
#define GRID_SIZE   32

#define OUT_OF_BOUNDS(s)    (s->x < 0 || s->y < 0 || s->x >= GRID_SIZE || s->y >= GRID_SIZE)

enum directions {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct vec2 {
    int x;
    int y;
};

struct snake {
    struct vec2 body[GRID_SIZE * GRID_SIZE];
    int length;
    int direction;
};

/**
 * Resets the snake to a single stationary segment at the center of the screen.
 */
static void reset_snake(struct snake *snake)
{
    snake->body[0].x = (GRID_SIZE / 2) - 1;
    snake->body[0].y = (GRID_SIZE / 2) - 1;
    snake->length = 1;
    snake->direction = NONE;
}

/**
 * Generates a new apple in a random cell provided the cell is not occupied by
 * the snake.
 */
static void random_apple(struct vec2 *apple, struct snake *snake)
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

/**
 * Updates the direction of the snake.
 */
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

/**
 * Moves the snake and checks if it has gone out of bounds, hit itself, or eaten
 * an apple.
 */
static void update(uint64_t dt, struct snake *snake, struct vec2 *apple)
{
    static const uint64_t interval = 80;
    static uint64_t elapsed = 0;
    elapsed += dt;
    while (elapsed > interval) {
        elapsed -= interval;

        /* Shift the snake segments to their predecessor's position. */
        int occupancy[GRID_SIZE][GRID_SIZE] = {0};
        for (int i = snake->length - 1; i > 0; --i) {
            snake->body[i] = snake->body[i - 1];
            occupancy[snake->body[i].x][snake->body[i].y] = 1;
        }

        /* Move the snake. */
        struct vec2 *head = &snake->body[0];
        switch (snake->direction) {
        case UP:
            --head->y;
            break;
        case DOWN:
            ++head->y;
            break;
        case LEFT:
            --head->x;
            break;
        case RIGHT:
            ++head->x;
            break;
        }

        /* Reset the snake if it goes out of bounds or hit its tail. */
        if (OUT_OF_BOUNDS(head) || occupancy[head->x][head->y] == 1) {
            reset_snake(snake);
            random_apple(apple, snake);
            return;
        }

        /* Grow the snake if it eats an apple. */
        occupancy[head->x][head->y] = 1;
        if (head->x == apple->x && head->y == apple->y) {
            random_apple(apple, snake);
            if (snake->length + 1 == GRID_SIZE * GRID_SIZE)
                return;

            snake->body[snake->length].x = snake->body[snake->length - 1].x;
            snake->body[snake->length].y = snake->body[snake->length - 1].y;
            ++snake->length;
        }
    }
}

/**
 * Draws the snake and apple to the screen.
 */
static void render(struct graphics *graphics, struct snake *snake, struct vec2 *apple)
{
    /* Draw the apple. */
    SDL_Rect apple_rect = {0, 0, ENTITY_SIZE, ENTITY_SIZE};
    apple_rect.x = apple->x * TILE_SIZE;
    apple_rect.y = apple->y * TILE_SIZE;
    SDL_SetRenderDrawColor(graphics->renderer, 0xAC, 0x38, 0x38, 0xFF);
    SDL_RenderFillRect(graphics->renderer, &apple_rect);

    /* Draw the snake. */
    SDL_Rect segment = {0, 0, ENTITY_SIZE, ENTITY_SIZE};
    for (int i = 0; i < snake->length; ++i) {
        segment.x = snake->body[i].x * TILE_SIZE;
        segment.y = snake->body[i].y * TILE_SIZE;
        SDL_SetRenderDrawColor(graphics->renderer, 0x40, 0x98, 0x5E, 0xFF);
        SDL_RenderFillRect(graphics->renderer, &segment);
    }
}

/**
 * Controls the main game loop.
 */
void game_loop(void)
{
    struct graphics *graphics = graphics_create();
    if (graphics == NULL)
        return;

    struct snake snake;
    reset_snake(&snake);

    struct vec2 apple;
    random_apple(&apple, &snake);

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
        update(now - timer, &snake, &apple);
        timer = now;

        SDL_SetRenderDrawColor(graphics->renderer, 0x11, 0x11, 0x11, 0xFF);
        SDL_RenderClear(graphics->renderer);
        render(graphics, &snake, &apple);
        SDL_RenderPresent(graphics->renderer);
    }
}
