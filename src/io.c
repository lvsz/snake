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

void print_field(Game *game)
{
    for (int i = 0; i < game->height; ++i) {
        for (int j = 0; j < game->width; ++j) {
            if (game->field[j][i] == '\0') {
                putchar(' ');
            } else {
                putchar(game->field[j][i]);
            }
        }
        putchar(10);
    }
}

void save(Game *game)
{
    FILE *savefile = fopen(SAVEFILE, "w");

    fprintf(savefile, "%lu;%lu;", game->width, game->height);
    for (int i = 0; i < game->width; ++i) {
        fwrite(game->field[i], sizeof(char), game->height, savefile);
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

    size_t width, height;
    fscanf(savefile, "%lu;%lu;", &width, &height);

    if (width != game->width || height != game->height) {
        window_resize(width, height);
        for (int i = 0; i < game->width; ++i) {
            free(game->field[i]);
        }
        free(game->field);

        game->width = width;
        game->height = height;
        game->field = malloc(width * sizeof(char *));
        for (int i = 0; i < width; ++i) {
            game->field[i] = malloc(height * sizeof(char));
        }
    }

    for (int i = 0; i < game->width; ++i) {
        fread(game->field[i], sizeof(char), game->height, savefile);
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

void handle_score(int score)
{
    HighScore *scores = get_scores();
    for (int i = 0; i < NR_OF_SCORES; ++i) {
        if (score > scores[i].score) {
            for (int j = NR_OF_SCORES - 1; j > i; --j) {
                scores[j] = scores[j - 1];
            }
            scores[i].score = score;
            char *name = get_name();
            strcpy(scores[i].name, name);
            free(name);
            write_scores(scores);
            break;
        }
    }
    free(scores);
}

