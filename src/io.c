#include "io.h"

Input read_input()
{
    SDL_Event event;

    int mouse_x, mouse_y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return QUIT;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        return P1_UP;
                    case SDLK_DOWN:
                        return P1_DOWN;
                    case SDLK_LEFT:
                        return P1_LEFT;
                    case SDLK_RIGHT:
                        return P1_RIGHT;
                    case SDLK_ESCAPE:
                    case SDLK_p:
                        return PAUSE;
                    case SDLK_s:
                        return SAVE;
                    case SDLK_l:
                        return LOAD;
                    default:
                        return NOTHING;
                }
            case SDL_MOUSEBUTTONDOWN:
                puts("mouseclick");
                break;
        }
    }
    return NOTHING;
}

void print_field(char **field)
{
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            if (field[j][i] == '\0') {
                putchar(' ');
            } else {
                putchar(field[j][i]);
            }
        }
        putchar(10);
    }
}

void save(Game *game)
{
    FILE *savefile = fopen(SAVEFILE, "w");

    for (int i = 0; i < FIELD_WIDTH; ++i) {
        fwrite(game->field[i], sizeof(char), FIELD_HEIGHT, savefile);
    }

    print_snake(savefile, game->snake);
    fclose(savefile);
}

void load(Game *game)
{
    puts("loading");
    FILE *savefile = fopen(SAVEFILE, "r");

    if (savefile == NULL) {
        fprintf(stderr, "Load error: %s not found.\n", SAVEFILE);
        return;
    }

    for (int i = 0; i < FIELD_WIDTH; ++i) {
        fread(game->field[i], sizeof(char), FIELD_HEIGHT, savefile);
    }

    Direction direction;
    int head_dif, tail_dif;
    fscanf(savefile, "%d;%d;%d;", &direction, &head_dif, &tail_dif);
    fread(game->snake->body, sizeof(Point), SNAKE_BUFFER, savefile);

    game->snake->direction = direction;
    game->snake->head = game->snake->body + head_dif;
    game->snake->tail = game->snake->body + tail_dif;

    fclose(savefile);
    puts("done loading");
}

