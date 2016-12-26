#include <stdlib.h>
#include <stdio.h>

#include "window.h"
#include "game.h"

int main(int argc, char *argv[])
{
    window_init("Snake");

    run_game();
    return 0;
}

