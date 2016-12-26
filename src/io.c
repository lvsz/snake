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
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            fputc(game->field[j][i], savefile);
        }
    }
    fprintf(savefile, "\n%lu %d\n", game->snake->size, game->snake->direction);
    print_snake(savefile, game->snake);
    fclose(savefile);
}

void load(Game *game)
{
    puts("loading");
    FILE *savefile = fopen(SAVEFILE, "r");

    if (savefile == NULL) {
        fprintf(stderr, "Load error: %s not found.", SAVEFILE);
        return;
    }

    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            game->field[j][i] = fgetc(savefile);
        }
    }
    size_t size;
    Direction direction;
    fscanf(savefile, "%lu %d", &size, &direction);
    printf("size: %lu, direction: %d\n", size, direction);

    game->snake->size = size;
    game->snake->direction = direction;
    game->snake->head = game->snake->body + size - 1;
    game->snake->tail = game->snake->body;

    for (size_t i = 0, x, y; i < size; ++i) {
        fscanf(savefile, "%lu %lu", &x, &y);
        printf("scanned (%lu, %lu)\n", x, y);
        (game->snake->body[i]).x = x;
        (game->snake->body[i]).y = y;
    }

    fclose(savefile);
    puts("done loading");
}

