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
void resize_game(Game *, size_t, size_t);

void new_food(Game *);
int is_food(Game *, Point);

#endif /* _GAME_H_ */

