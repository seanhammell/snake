#include <time.h>
#include <stdlib.h>

#include "src/game.h"

int main(void)
{
    srand(time(NULL));
    game_loop();
    return 0;
}
