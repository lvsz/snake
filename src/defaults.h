#ifndef _DEFAULTS_H_
#define _DEFAULTS_H_

#define SAVEFILE "save.snk"
#define SCOREFILE "highscores.snk"
#define FONTFILE "../Resources/Font.ttf"

#define BLOCK_SIZE 40

#define FIELD_WIDTH 20
#define FIELD_HEIGHT 15

#define NR_OF_SCORES (FIELD_HEIGHT - 1)

#define WINDOW_WIDTH (FIELD_WIDTH * BLOCK_SIZE)
#define WINDOW_HEIGHT (FIELD_HEIGHT * BLOCK_SIZE)

#define SNAKE_SIZE 3
#define SNAKE_START_X SNAKE_SIZE
#define SNAKE_START_Y (FIELD_HEIGHT / 2)
#define SNAKE_BUFFER (FIELD_WIDTH * FIELD_HEIGHT)

#endif /* _DEFAULTS_H_ */

