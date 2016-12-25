#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <stdlib.h>

#include "queue.h"
#include "types.h"

typedef Queue Snake;

Snake *new_snake();
void free_snake(Snake *);

Point head_point(Snake *);
void add_head(Snake *, Point);
Point pop_tail(Snake *);

#endif /* _SNAKE_H_ */

