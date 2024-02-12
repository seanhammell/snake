#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_

#define TILE_SIZE 16
#define GRID_SIZE 32
#define SCREEN_SIZE (TILE_SIZE * GRID_SIZE)

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  NONE,
} Directions;

#endif  /* SRC_CONSTANTS_H_ */
