#ifndef _DEFAULTS_H_
#define _DEFAULTS_H_

/* title to show in the window's title bar */
#define TITLE "Snake"

/* locations of files that are used */
#define SAVEFILE "save.snk"
#define SCOREFILE "highscores.snk"
#define LEVELFILE "levels/level"
#define FONTFILE "Resources/fonts/SourceCodePro-Bold.ttf"

/* length of a block's side in pixels
 * increasing this will also increase the window's size */
#define BLOCK_SIZE 25

/* nr of scores to save and load from a highscore file */
#define NR_OF_SCORES 25

/* nr of ms between gameloop updates */
#define DEFAULT_SPEED 120

/* starting size when placing a new snake */
#define SNAKE_SIZE 3
#define SNAKE_BUFFER 1000

/* points special food gives
 * turns it takes until it appears
 * and turns it takes until it dissapears */
#define TREAT_POINTS 5
#define TURNS_BETWEEN_TREATS 50
#define TURNS_TO_GET_TREAT 15

/* score needed to progress to the next level */
#define TARGET_SCORE (int) (game->width * game->height / 8)

#define P1_START (Point) { game->width / 2, game->height / 2 }
#define P2_START (Point) { game->width / 2, game->height / 2 + 1 }

#endif /* _DEFAULTS_H_ */

