#ifndef _IO_H_
#define _IO_H_

#include "SDL/SDL.h"

#include "game.h"
#include "window.h"
#include "snake.h"
#include "types.h"
#include "defaults.h"

Input read_input();

void print_point(void *);
void save(Game *);
void load(Game *);
void load_level(Game *, int);

HighScore *get_scores();
void write_scores(HighScore *);
void handle_score(int);

#endif /* _IO_H_ */

