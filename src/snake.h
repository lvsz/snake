#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "defaults.h"

Snake *new_snake(Point *, size_t, Direction);
void free_snake(Snake *);

Point snake_head(Snake *);
void add_head(Snake *, Point);
Point pop_tail(Snake *);

void print_snake(FILE *, Snake *);

#endif /* _SNAKE_H_ */

