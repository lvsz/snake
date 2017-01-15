#ifndef _DEFAULTS_H_
#define _DEFAULTS_H_

#define TITLE "Snake"

#define SAVEFILE "save.snk"
#define SCOREFILE "highscores.snk"
#define LEVELFILE "../levels/level"
#define FONTFILE "../Resources/Font.ttf"

#define BLOCK_SIZE 25

#define FIELD_WIDTH 20
#define FIELD_HEIGHT 15

#define NR_OF_SCORES 25

#define DEFAULT_SPEED 120

#define SNAKE_SIZE 3
#define SNAKE_BUFFER 1000

#define TREAT_POINTS 5
#define TURNS_BETWEEN_TREATS 50
#define TURNS_TO_GET_TREAT 15

#define TARGET_SCORE (int) (game->width * game->height / 8)

#define P1_START (Point) { game->width / 2, game->height / 2 }
#define P2_START (Point) { game->width / 2, game->height / 2 + 1 }

#endif /* _DEFAULTS_H_ */

