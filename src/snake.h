#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <stdlib.h>
#include <stdio.h>

#include "game.h"
#include "types.h"
#include "defaults.h"

Snake *new_snake(Direction);
void free_snake(Snake *);
void place_snake(Game *, size_t, size_t, Direction);
int update_snake(Game *);

Point snake_head(Snake *);
void add_head(Snake *, Point);
Point pop_tail(Snake *);

void print_snake(FILE *, Snake *);

#endif /* _SNAKE_H_ */

