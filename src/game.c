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
    game->level = 0;
    game->speed = DEFAULT_SPEED;
    game->turns = 0;
    game->total_score = 0;
    game->players = 1;
    game->treat = NULL;
    game->p2 = NULL;

    if (level >= 0) {
        load_level(game, level);
    } else {
        place_snake(game, 1, P1_START, RIGHT);
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
    free(game->p1);
    free(game->p2);
    puts("freeing field");
    free(game->field);
    puts("freeing game");
    free(game);
    puts("game succesfully freed");
}

void clear_game(Game *game)
{
    for (int i = 0; i < game->width; ++i) {
        for (int j = 0; j < game->height; ++j) {
            game->field[i][j] = '\0';
        }
    }

    game->level = 0;
    game->total_score = 0;
    game->turns = 0;
    game->speed = DEFAULT_SPEED;
    game->treat = NULL;
    free(game->p1);
    place_snake(game, 1, P1_START, RIGHT);
    if (game->players > 1) {
        free(game->p2);
        place_snake(game, 2, P2_START, LEFT);
    }

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

void toggle_multiplayer(Game *game)
{
    if (game->players > 1) {
        game->players = 1;
        clear_snake(game, game->p2);
        game->p2 = NULL;
    } else {
        game->players = 2;
        place_snake(game, 2, P2_START, LEFT);
    }
}

int pause(Game *game)
{
    puts("pause");
    window_pause();
    Input input;
    while (1) {
        switch (input = read_input()) {
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
            case MULTIPLAYER:
                toggle_multiplayer(game);
                break;
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
                break;
            default:
                break;
        }

        draw_field(game);
        delay(20);
    }
}

int next_level(Game *game)
{
    if (game->level >= 1
            && game->level <= 9
            && game->players == 1
            && game->p1->score >= TARGETSCORE) {
        int score = game->total_score +  game->p1->score;
        int next = game->level + 1;
        clear_game(game);
        load_level(game, next);
        game->total_score = score;
        return 1;
    } else {
        return 0;
    }
}

int run_game(Game *game)
{
    clear_screen();
    int playing = pause(game);
    if (!playing)
        return 0;

    Input input;
    while (playing) {
        draw_field(game);
        switch (input = read_input()) {
            case NOTHING:
                break;
            case P1_UP:
                if (game->p1->direction != DOWN)
                    game->p1->direction = UP;
                break;
            case P1_DOWN:
                if (game->p1->direction != UP)
                    game->p1->direction = DOWN;
                break;
            case P1_LEFT:
                if (game->p1->direction != RIGHT)
                    game->p1->direction = LEFT;
                break;
            case P1_RIGHT:
                if (game->p1->direction != LEFT)
                    game->p1->direction = RIGHT;
                break;
            case P2_UP:
                if (game->players > 1 && game->p2->direction != DOWN)
                    game->p2->direction = UP;
                break;
            case P2_DOWN:
                if (game->players > 1 && game->p2->direction != UP)
                    game->p2->direction = DOWN;
                break;
            case P2_LEFT:
                if (game->players > 1 && game->p2->direction != RIGHT)
                    game->p2->direction = LEFT;
                break;
            case P2_RIGHT:
                if (game->players > 1 && game->p2->direction != LEFT)
                    game->p2->direction = RIGHT;
                break;
            case SAVE:
                save(game);
                break;
            case LOAD:
                load(game);
                if (pause(game))
                    break;
                else
                    return 0;
                break;
            case MULTIPLAYER:
                toggle_multiplayer(game);
                break;
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
                if (pause(game))
                    break;
                else
                    return 0;
            case PAUSE:
                if (pause(game))
                    break;
            case QUIT:
                return 0;
        }

        if (game->players > 1) {
            move_snake(game, game->p1);
            move_snake(game, game->p2);
            playing &= check_snake(game, game->p1);
            playing &= check_snake(game, game->p2);
        } else {
            move_snake(game, game->p1);
            playing &= check_snake(game, game->p1);
        }

        game->turns++;
        if (game->turns % TURNS_BETWEEN_TREATS == 0) {
            new_food(game, TREAT);
        } else if (game->turns % TURNS_BETWEEN_TREATS == TURNS_TO_GET_TREAT) {
            remove_treat(game);
        }

        delay(game->speed);
    }

    draw_field(game);
    if (next_level(game)) {
        return 1;
    } else if (handle_score(game)) {
        clear_game(game);
        new_food(game, FOOD);
        return game->players > 1 ? 1 : score_screen();
    } else {
        return 0;
    }
}

