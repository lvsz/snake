#include "window.h"
#include "game.h"

int main(int argc, char *argv[])
{
    Game *game;
    size_t game_width, game_height;

    /* if arguments are given, start in custom field
     * otherwise load level 1 and start there */
    if (argc >= 3) {
        game_width = atoi(argv[1]);
        game_height = atoi(argv[2]);
        game = game_init(game_width, game_height, -1);
    } else {
        /* when loading a level, width and height parameters
         * are 0 to avoid allocating until reading the file */
        game = game_init(0, 0, 1);
        game_width = game->width;
        game_height = game->height;
    }

    window_init(game_width, game_height);

    /* keep game running until it returns 0 */
    while(run_game(game));

    free_game(game);
    return 0;
}

