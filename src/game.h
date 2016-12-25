#ifndef _GAME_H_
#define _GAME_H_

#include <stdlib.h>
#include <time.h>

#include "snake.h"
#include "window.h"
#include "input.h"
#include "defaults.h"
#include "types.h"

typedef enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

void run_game();

#endif /* _GAME_H_ */

