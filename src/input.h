#ifndef _INPUT_H_
#define _INPUT_H_

#include "SDL/SDL.h"

typedef enum Input {
    P1_UP,
    P1_DOWN,
    P1_LEFT,
    P1_RIGHT,
    PAUSE,
    SAVE,
    QUIT,
    NOTHING
} Input;

Input read_input();

#endif /* _INPUT_H_ */

