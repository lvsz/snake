#ifndef _INPUT_H_
#define _INPUT_H_

#include "SDL/SDL.h"

#include "snake.h"
#include "types.h"
#include "defaults.h"

typedef enum Input {
    QUIT,
    P1_UP,
    P1_DOWN,
    P1_LEFT,
    P1_RIGHT,
    PAUSE,
    SAVE,
    LOAD,
    NOTHING
} Input;

Input read_input();

void print_field(char **);
void print_point(void *);
void save(Game *);
void load(Game *);

#endif /* _INPUT_H_ */

