#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include "io.h"
#include "defaults.h"

void window_init(char *);
void clear_screen();

void draw_field(char **);

int score_screen();
char *get_name();

#endif /* _WINDOW_H_ */

