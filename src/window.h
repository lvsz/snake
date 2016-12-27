#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include "defaults.h"

void window_init(char *);

void draw_field(char **);

void draw_scores();
char *get_name();

#endif /* _WINDOW_H_ */

