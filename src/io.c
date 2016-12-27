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
                printf("%d: ", event.key.keysym.sym);
                puts(SDL_GetKeyName(event.key.keysym.sym));
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

    fprintf(savefile, "%d;", game->score);
    print_snake(savefile, game->snake);
    fclose(savefile);
}

void load(Game *game)
{
    puts("loading");
    FILE *savefile = fopen(SAVEFILE, "r");

    if (savefile == NULL) {
        fprintf(stderr, "Error: %s not found.\n", SAVEFILE);
        return;
    }

    for (int i = 0; i < FIELD_WIDTH; ++i) {
        fread(game->field[i], sizeof(char), FIELD_HEIGHT, savefile);
    }

    Direction direction;
    int head_dif, tail_dif;
    fscanf(savefile, "%d;%d;", &(game->score), &direction);
    fscanf(savefile, "%d;%d;", &head_dif, &tail_dif);
    fread(game->snake->body, sizeof(Point), SNAKE_BUFFER, savefile);

    game->snake->direction = direction;
    game->snake->head = game->snake->body + head_dif;
    game->snake->tail = game->snake->body + tail_dif;

    fclose(savefile);
    puts("done loading");
}

void create_scorefile()
{
    HighScore *scores = calloc(NR_OF_SCORES, sizeof(HighScore));

    for (int i = 0; i < NR_OF_SCORES; ++i) {
        strcpy(scores[i].name, "___");
    }

    FILE *scorefile = fopen(SCOREFILE, "w");

    if (scorefile == NULL) {
        fprintf(stderr, "Error: %s cannot be created\n", SCOREFILE);
        exit(1);
    }

    fwrite(scores, sizeof(HighScore), NR_OF_SCORES, scorefile);
    fclose(scorefile);
    free(scores);
}

HighScore *get_scores()
{
    HighScore *scores = malloc(NR_OF_SCORES * sizeof(HighScore));

    FILE *scorefile = fopen(SCOREFILE, "r");
    if (scorefile == NULL) {
        create_scorefile();
        scorefile = fopen(SCOREFILE, "r");
        if (scorefile == NULL) {
            fprintf(stderr, "Error: %s cannot be opened\n", SCOREFILE);
            exit(1);
        }
    }

    fread(scores, sizeof(HighScore), NR_OF_SCORES, scorefile);
    fclose(scorefile);
    return scores;
}

void write_scores(HighScore *scores)
{
    FILE *scorefile = fopen(SCOREFILE, "w");
    if (scorefile == NULL) {
        fprintf(stderr, "Error: %s cannot be edited\n", SCOREFILE);
        exit(1);
    }

    fwrite(scores, sizeof(HighScore), NR_OF_SCORES, scorefile);
    fclose(scorefile);
}

