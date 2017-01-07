#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include "io.h"
#include "types.h"
#include "defaults.h"

void window_init(size_t, size_t);
void window_quit();
void window_resize(size_t, size_t);
void window_pause();

void clear_screen();
void draw_field(Game *);

int score_screen();
char *get_name();

#endif /* _WINDOW_H_ */

