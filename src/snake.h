#ifndef SRC_SNAKE_H_
#define SRC_SNAKE_H_

#ifndef SNAKE_
typedef void Snake;
#endif  /* SNAKE_ */

Snake *SnakeCreate(void);
void SnakeDestroy(Snake *snake);

#endif  /* SRC_SNAKE_H_ */
