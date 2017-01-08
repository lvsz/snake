#include "game.h"

Game *game_init(size_t game_width, size_t game_height, int level)
{
    Game *game = malloc(sizeof(Game));
    if (game == NULL) {
        fprintf(stderr, "Error: game allocation failed\n");
        exit(1);
    }

    if ((game->field = malloc(game_width * sizeof(char *))) == NULL) {
        fprintf(stderr, "Error: game allocation failed\n");
        exit(1);
    }

    for (size_t i = 0; i < game_width; ++i) {
        if ((game->field[i] = calloc(game_height, sizeof(char))) == NULL) {
            fprintf(stderr, "Error: game allocation failed\n");
            exit(1);
        }
    }

    game->width = game_width;
    game->height = game_height;
    game->score = 0;
    game->turns = 0;
    game->treat = NULL;

    if (level >= 0) {
        load_level(game, level);
    } else {
        place_snake(game, game->width / 2, game->height / 2, RIGHT);
        new_food(game, FOOD);
    }

    return game;
}

void free_game(Game *game)
{

    for (size_t i = 0; i < game->width; ++i) {
        free((game->field)[i]);
    }

    puts("freeing snake");
    free_snake(game->snake);
    puts("freeing field");
    free(game->field);
    puts("freeing game");
    free(game);
    puts("game succesfully freed");
}

void clear_game(Game *game)
{
    game->score = 0;
    for (int i = 0; i < game->width; ++i) {
        for (int j = 0; j < game->height; ++j) {
            game->field[i][j] = '\0';
        }
    }

    game->turns = 0;
    game->treat = NULL;
    free_snake(game->snake);
    place_snake(game, game->width / 2, game->height / 2, RIGHT);
    clear_screen();
}

void resize_game(Game *game, size_t width, size_t height)
{
    window_resize(width, height);

    for (int i = 0; i < game->width; ++i) {
        free(game->field[i]);
    }

    free(game->field);

    game->width = width;
    game->height = height;
    game->field = malloc(width * sizeof(char *));

    for (int i = 0; i < width; ++i) {
        game->field[i] = calloc(height, sizeof(char));
    }

    draw_field(game);
}

int pause(Game *game)
{
    puts("pause");
    window_pause();
    while (1) {
        switch (read_input()) {
            case PAUSE:
                return 1;
            case QUIT:
                return 0;
            case SAVE:
                save(game);
                break;
            case LOAD:
                load(game);
                break;
            default:
                break;
        }

        draw_field(game);
        SDL_Delay(20);
    }
}

void new_food(Game *game, Food type)
{
    srand(time(NULL));
    size_t x = rand() % game->width;
    size_t y = rand() % game->height;

    while (game->field[x][y] != '\0') {
        x = rand() % game->width;
        y = rand() % game->height;
    }

    if (type == TREAT) {
        printf("placed treat at (%lu, %lu)\n", x, y);
        game->field[x][y] = 't';
        game->treat = &(game->field[x][y]);
    } else {
        printf("placed food at (%lu, %lu)\n", x, y);
        game->field[x][y] = 'f';
    }
}

int is_food(Game *game, Point p)
{
    return game->field[p.x][p.y] == 'f';
}

int is_treat(Game *game, Point p)
{
    return game->field[p.x][p.y] == 't';
}

void remove_treat(Game *game)
{
    if (game->treat != NULL && *(game->treat) == 't') {
       *(game->treat) = '\0'; 
    }
    game->treat = NULL;
}

int run_game(Game *game)
{
    puts("starting");
    clear_screen();
    int playing = 1;
    int paused = 0;
    Input input;

    while (playing) {
        draw_field(game);
        puts("reading input");
        switch (input = read_input()) {
            case NOTHING:
                break;
            case P1_UP:
                if (game->snake->direction != DOWN)
                    game->snake->direction = UP;
                break;
            case P1_DOWN:
                if (game->snake->direction != UP)
                    game->snake->direction = DOWN;
                break;
            case P1_LEFT:
                if (game->snake->direction != RIGHT)
                    game->snake->direction = LEFT;
                break;
            case P1_RIGHT:
                if (game->snake->direction != LEFT)
                    game->snake->direction = RIGHT;
                break;
            case SAVE:
                save(game);
                break;
            case LOAD:
                load(game);
                draw_field(game);
                return pause(game);
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                clear_game(game);
                load_level(game, input);
                draw_field(game);
                return pause(game);
            case PAUSE:
                paused ^= 1;
                if (pause(game))
                    break;
            case QUIT:
                return 0;
        }

        playing = update_snake(game);
        game->turns++;
        if (game->turns % TURNS_BETWEEN_TREATS == 0) {
            new_food(game, TREAT);
        } else if (game->turns % TURNS_BETWEEN_TREATS == TURNS_TO_GET_TREAT) {
            remove_treat(game);
        }
        printf("finished turn %d\n", game->turns);
        SDL_Delay(100);
    }

    if (handle_score(game->score)) {
        clear_game(game);
        new_food(game, FOOD);
        return score_screen();
    } else {
        return 0;
    }
}

