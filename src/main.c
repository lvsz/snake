#include <stdlib.h>
#include <stdio.h>

#include "window.h"
#include "game.h"

int main(int argc, char *argv[])
{
    Game *game;
    size_t game_width, game_height;
    if (argc >= 3) {
        game_width = atoi(argv[1]);
        game_height = atoi(argv[2]);
        game = game_init(game_width, game_height, -1);
    } else {
        game = game_init(0, 0, 0);
        game_width = game->width;
        game_height = game->height;
    }

    window_init(game_width, game_height);

    puts("starting game");
    while(run_game(game));

    puts("exiting");
    window_quit();
    free_game(game);
    return 0;
}

