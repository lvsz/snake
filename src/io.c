#include "io.h"

Input read_input()
{
    int mouse_x, mouse_y;

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
            case SDL_MOUSEBUTTONDOWN:
                puts("mouseclick");
                break;
        }
    }
    return NOTHING;
}

void print_snake(FILE *stream, Snake *snake)
{
    putc(snake->player, stream);
    fprintf(stream, "%d;", snake->score);
    fprintf(stream, "%d;", snake->direction);
    fprintf(stream, "%d;", (int) (snake->head - snake->body));
    fprintf(stream, "%d;", (int) (snake->tail - snake->body));
    fwrite(snake->body, sizeof(Point), SNAKE_BUFFER, stream);
}

void save(Game *game)
{
    FILE *savefile = fopen(SAVEFILE, "w");

    fprintf(savefile, "%lu;%lu;", game->width, game->height);
    for (int i = 0; i < game->width; ++i) {
        fwrite(game->field[i], sizeof(char), game->height, savefile);
    }

    fprintf(savefile, "%d;%d;", game->players, game->turns);
    print_snake(savefile, game->p1);
    if (game->players > 1) {
        print_snake(savefile, game->p2);
    }

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
        resize_game(game, width, height);
    }

    game->treat = NULL;
    for (int i = 0; i < game->width; ++i) {
        fread(game->field[i], sizeof(char), game->height, savefile);
        if (game->treat == NULL) {
            game->treat = memchr(game->field[i], 't', game->height);
        }
    }

    Direction direction;
    int players, head_dif, tail_dif;
    fscanf(savefile, "%d;%d;", &players, &(game->turns));
    if (players < game->players) {
        clear_snake(game, game->p2);
        game->p2 = NULL;
    }

    game->players = players;
    game->p1->player = getc(savefile);
    fscanf(savefile, "%d;%d;", &(game->p1->score), &(game->p1->direction));
    fscanf(savefile, "%d;%d;", &head_dif, &tail_dif);
    fread(game->p1->body, sizeof(Point), SNAKE_BUFFER, savefile);
    game->p1->head = game->p1->body + head_dif;
    game->p1->tail = game->p1->body + tail_dif;

    if (game->players > 1) {
        if (game->p2 == NULL)
            game->p2 = malloc(sizeof(Snake));

        game->p2->player = getc(savefile);
        fscanf(savefile, "%d;%d;", &(game->p2->score), &(game->p2->direction));
        fscanf(savefile, "%d;%d;", &head_dif, &tail_dif);
        fread(game->p2->body, sizeof(Point), SNAKE_BUFFER, savefile);
        game->p2->head = game->p2->body + head_dif;
        game->p2->tail = game->p2->body + tail_dif;
    }

    fclose(savefile);
    puts("done loading");

    clear_screen();
    draw_field(game);
}

void load_level(Game *game, int n)
{
    char filename[sizeof(LEVELFILE) + 1];
    sprintf(filename, "%s%d", LEVELFILE, n);

    FILE *levelfile = fopen(filename, "r");
    if (levelfile == NULL) {
        printf("%s not found\n", filename);
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
    draw_field(game);
}

void create_scorefile()
{
    HighScore scores[NR_OF_SCORES];

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
    int score = game->p1->score;
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

