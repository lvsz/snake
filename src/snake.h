#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <stdlib.h>
#include <stdio.h>

#include "game.h"
#include "types.h"
#include "defaults.h"

void place_snake(Game *, int, Point, Direction);
void clear_snake(Game *, Snake *);
void move_snake(Game *, Snake *);
int check_snake(Game *, Snake *);

Point snake_head(Snake *);
void add_head(Snake *, Point);
Point pop_tail(Snake *);

#endif /* _SNAKE_H_ */

