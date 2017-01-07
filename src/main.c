#include <stdlib.h>
#include <stdio.h>

#include "window.h"
#include "game.h"

int main(int argc, char *argv[])
{
    size_t game_width, game_height;
    if (argc >= 3) {
        game_width = atoi(argv[1]);
        game_height = atoi(argv[2]);
    } else {
        game_width = 15;
        game_height = 20;
    }

    window_init(game_width, game_height);
    Game *game = game_init(game_width, game_height);

    while(run_game(game));

    puts("exiting");
    window_quit();
    free_game(game);
    return 0;
}

