#include "game.h"

void place_snake(Game *game, size_t start_x, size_t start_y, Direction direction)
{
    if (direction != LEFT && direction != RIGHT) {
        fprintf(stderr, "Invalid intial direction, defaulting to RIGHT\n");
        direction = RIGHT;
    }

    game->snake = new_snake(direction);

    for (int i = SNAKE_SIZE - 1; i >= 0; --i) {
        size_t x = direction == LEFT ? start_x + i : start_x - i + 1;
        Point p = {x, start_y};
        add_head(game->snake, p);
        game->field[x][start_y] = 's';
    }
}

Game *game_init(size_t game_width, size_t game_height)
{
    Game *game;
    if ((game  = malloc(sizeof(Game))) == NULL
       || (game->field = malloc(game_width * sizeof(char *))) == NULL) {
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
    place_snake(game, game->width / 2, game->height / 2, RIGHT);

    clear_screen();
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
    free_snake(game->snake);
    place_snake(game, game->width / 2, game->height / 2, RIGHT);
    clear_screen();
}

void pause()
{
    puts("pause");
    while (read_input() != PAUSE) {
        SDL_Delay(100);
    }
}

int is_food(Game *game, Point p)
{
    return game->field[p.x][p.y] == 'f';
}

void new_food(Game *game)
{
    int tries = 1;
    srand(time(NULL));
    size_t x = rand() % game->width;
    size_t y = rand() % game->height;

    while (game->field[x][y] != '\0') {
        x = rand() % game->width;
        y = rand() % game->height;
        ++tries;
    }

    printf("placed food at (%lu, %lu)\n", x, y);
    printf("tries: %d\n", tries);
    game->field[x][y] = 'f';
}

int update_snake(Game *game)
{
    Point head = snake_head(game->snake);

    switch (game->snake->direction) {
        case UP:
            head.y = (head.y > 0 ? head.y : game->height) - 1;
            break;
        case DOWN:
            head.y = (head.y + 1) % game->height;
            break;
        case LEFT:
            head.x = (head.x > 0 ? head.x : game->width) - 1;
            break;
        case RIGHT:
            head.x = (head.x + 1) % game->width;
            break;
    }

    if (is_food(game, head)) {
        add_head(game->snake, head);
        game->field[head.x][head.y] = 's';
        ++(game->score);
        new_food(game);
        return 1;
    } else {
        Point tail = pop_tail(game->snake);
        game->field[tail.x][tail.y] = '\0';

        switch (game->field[head.x][head.y]) {
            case 's':
                puts("game over");
                return 0;
            default:
                add_head(game->snake, head);
                game->field[head.x][head.y] = 's';
                return 1;
        }
    }
}

int run_game(Game *game)
{
    new_food(game);

    puts("starting");
    clear_screen();
    int playing = 1;
    int paused = 0;

    while (playing) {
        draw_field(game);
        switch (read_input()) {
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
            case PAUSE:
                paused ^= 1;
                pause();
                break;
            case SAVE:
                save(game);
                break;
            case LOAD:
                load(game);
                break;
            case QUIT:
                free_game(game);
                return 0;
        }

        playing = update_snake(game);
        SDL_Delay(100);
    }

    handle_score(game->score);
    clear_game(game);
    return score_screen();
}

