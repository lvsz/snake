#ifndef _GAME_H_
#define _GAME_H_

#include <stdlib.h>
#include <time.h>

#include "window.h"
#include "io.h"
#include "snake.h"
#include "types.h"
#include "defaults.h"

int run_game(Game *);
Game *game_init(size_t, size_t);
void free_game(Game *);

#endif /* _GAME_H_ */

