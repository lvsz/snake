#include "io.h"

Input read_input()
{
    SDL_Event event;
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
                    case SDLK_w:
                        return P2_UP;
                    case SDLK_s:
                        return P2_DOWN;
                    case SDLK_a:
                        return P2_LEFT;
                    case SDLK_d:
                        return P2_RIGHT;
                    case SDLK_c:
                        return SAVE;
                    case SDLK_v:
                        return LOAD;
                    case SDLK_ESCAPE:
                    case SDLK_SPACE:
                    case SDLK_p:
                        return PAUSE;
                    case SDLK_m:
                        return MULTIPLAYER;
                    case SDLK_0:
                    case SDLK_1:
                    case SDLK_2:
                    case SDLK_3:
                    case SDLK_4:
                    case SDLK_5:
                    case SDLK_6:
                    case SDLK_7:
                    case SDLK_8:
                    case SDLK_9:
                        return event.key.keysym.sym - '0';
                    default:
                        return NOTHING;
                }

            default:
                break;
        }
    }

    return NOTHING;
}

/* used to save the Snake struct to a file */
void print_snake(FILE *stream, Snake *snake)
{
    putc(snake->player, stream);
    fprintf(stream, "%d;", snake->score);
    fprintf(stream, "%u;", snake->direction);
    fprintf(stream, "%d;", (int) (snake->head - snake->body));
    fprintf(stream, "%d;", (int) (snake->tail - snake->body));
    fwrite(snake->body, sizeof(Point), SNAKE_BUFFER, stream);
}

/* saves the current state of the game to a file */
void save(Game *game)
{
    FILE *savefile = fopen(SAVEFILE, "w");

    fprintf(savefile, "%lu;%lu;", game->width, game->height);
    for (int i = 0; i < game->width; ++i) {
        fwrite(game->field[i], sizeof(char), game->height, savefile);
    }

    fprintf(savefile, "%d;", game->level);
    fprintf(savefile, "%d;", game->total_score);
    fprintf(savefile, "%d;", game->turns);
    fprintf(savefile, "%d;", game->speed);
    fprintf(savefile, "%d;", game->players);
    print_snake(savefile, game->p1);
    if (game->players > 1) {
        print_snake(savefile, game->p2);
    }

    fclose(savefile);
}

/* load a previously saved game */
void load(Game *game)
{
    FILE *savefile = fopen(SAVEFILE, "r");

    if (savefile == NULL) {
        fprintf(stderr, "Error: %s not found.\n", SAVEFILE);
        return;
    }

    size_t width, height;
    fscanf(savefile, "%lu;%lu;", &width, &height);

    if (width != game->width || height != game->height) {
        resize_game(game, width, height);
    }

    game->treat = NULL;
    for (int i = 0; i < game->width; ++i) {
        fread(game->field[i], sizeof(char), game->height, savefile);
        if (game->treat == NULL) {
            game->treat = memchr(game->field[i], 't', game->height);
        }
    }

    int players;
    fscanf(savefile, "%d;", &(game->level));
    fscanf(savefile, "%d;", &(game->total_score));
    fscanf(savefile, "%d;", &(game->turns));
    fscanf(savefile, "%d;", &(game->speed));
    fscanf(savefile, "%d;", &players);
    if (players < game->players) {
        clear_snake(game, game->p2);
        game->p2 = NULL;
    }

    Direction direction;
    int head_dif, tail_dif;
    game->players = players;
    game->p1->player = getc(savefile);
    fscanf(savefile, "%d;%u;", &(game->p1->score), &(game->p1->direction));
    fscanf(savefile, "%d;%d;", &head_dif, &tail_dif);
    fread(game->p1->body, sizeof(Point), SNAKE_BUFFER, savefile);
    game->p1->head = game->p1->body + head_dif;
    game->p1->tail = game->p1->body + tail_dif;

    if (game->players > 1) {
        if (game->p2 == NULL)
            game->p2 = malloc(sizeof(Snake));

        game->p2->player = getc(savefile);
        fscanf(savefile, "%d;%u;", &(game->p2->score), &(game->p2->direction));
        fscanf(savefile, "%d;%d;", &head_dif, &tail_dif);
        fread(game->p2->body, sizeof(Point), SNAKE_BUFFER, savefile);
        game->p2->head = game->p2->body + head_dif;
        game->p2->tail = game->p2->body + tail_dif;
    }

    fclose(savefile);
    clear_screen();
}

/* load a level from a level file */
void load_level(Game *game, int n)
{
    game->level = n;

    char filename[sizeof(LEVELFILE) + 1];
    sprintf(filename, "%s%d", LEVELFILE, n);

    FILE *levelfile = fopen(filename, "r");
    if (levelfile == NULL) {
        fprintf(stderr, "%s not found\n", filename);
        return;
    }

    int players;
    size_t width, height;
    fscanf(levelfile, "%d %d", &players, &(game->speed));
    fscanf(levelfile, "%lu %lu", &width, &height);
    fseek(levelfile, 1, SEEK_CUR);

    if (players < game->players) {
        clear_snake(game, game->p2);
        game->p2 = NULL;
        game->players = players;
    }

    if (game->width != width || game->height != height) {
        resize_game(game, width, height);
    }

    char *row = malloc((width + 2) * sizeof(char));
    for (int y = 0; y < height; ++y) {
        if (fgets(row, width + 2, levelfile) == NULL) {
            break;
        }

        for (int x = 0; row[x] != '\0' && row[x] != '\n'; ++x) {
            if (row[x] != ' ') {
                game->field[x][y] = row[x];
            }
        }
    }

    free(row);
    fclose(levelfile);

    place_snake(game, 1, P1_START, RIGHT);
    new_food(game, FOOD);

    clear_screen();
}

/* create an "empty" score file when none found */
void create_scorefile()
{
    HighScore scores[NR_OF_SCORES];

    for (int i = 0; i < NR_OF_SCORES; ++i) {
        scores[i].score = 0;
        strcpy(scores[i].name, "___");
    }

    FILE *scorefile = fopen(SCOREFILE, "w");

    if (scorefile == NULL) {
        fprintf(stderr, "Error: %s cannot be created\n", SCOREFILE);
        exit(1);
    }

    fwrite(scores, sizeof(HighScore), NR_OF_SCORES, scorefile);
    fclose(scorefile);
}

/* read and return highscores from a file */
HighScore *get_scores()
{
    HighScore *scores = malloc(NR_OF_SCORES * sizeof(HighScore));

    FILE *scorefile = fopen(SCOREFILE, "r");
    if (scorefile == NULL) {
        create_scorefile(); /* create one when not found */
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

/* write highscore to a file */
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

/* check the score and add it to the highscores when needed
 * returns 1 to on success
 * returns 0 when player hit quit */
int handle_score(Game *game)
{
    if (game->players > 1) {
        Point head1 = snake_head(game->p1);
        Point head2 = snake_head(game->p2);
        if ((head1.x == head2.x && head1.y == head2.y)
           || game->p1->score == game->p2->score) {
            return show_winner(0);
        } else {
            return show_winner(game->p1->score < 0 ? 2 : 1);
        }
    }

    HighScore *scores = get_scores();
    int score = game->total_score + game->p1->score;
    for (int i = 0; i < NR_OF_SCORES; ++i) {
        if (score > scores[i].score) {
            for (int j = NR_OF_SCORES - 1; j > i; --j) {
                scores[j] = scores[j - 1];
            }
            scores[i].score = score;
            char *name = get_name();
            if (name == NULL) {
                free(scores);
                return 0;
            } else if (name[0] == '_') {
                /* player did not enter a name, so don't add it to file */
                free(name);
                free(scores);
                return 1;
            }

            strcpy(scores[i].name, name);
            free(name);
            write_scores(scores);
            break;
        }
    }

    free(scores);
    return 1;
}

