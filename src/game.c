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

Game *game_init()
{
    Game *game;
    if ((game  = malloc(sizeof(Game))) == NULL
       || (game->field = malloc(FIELD_WIDTH * sizeof(char *))) == NULL) {
        fprintf(stderr, "Error: game allocation failed\n");
        exit(1);
    }

    for (size_t i = 0; i < FIELD_WIDTH; ++i) {
        (game->field)[i] = calloc(FIELD_HEIGHT, sizeof(char));
    }

    game->score = 0;
    place_snake(game, SNAKE_START_X, SNAKE_START_Y, RIGHT);

    return game;
}

void free_game(Game *game)
{

    for (size_t i = 0; i < FIELD_WIDTH; ++i) {
        free((game->field)[i]);
    }

    free_snake(game->snake);
    free(game->field);
    free(game);
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
    srand(time(NULL));
    size_t x = rand() % FIELD_WIDTH;
    size_t y = rand() % FIELD_HEIGHT;

    while (game->field[x][y] != '\0') {
        x = rand() % FIELD_WIDTH;
        y = rand() % FIELD_HEIGHT;
    }

    printf("placed food at (%lu, %lu)\n", x, y);
    game->field[x][y] = 'f';
}

int update_snake(Game *game)
{
    Point head = snake_head(game->snake);

    switch (game->snake->direction) {
        case UP:
            head.y = (head.y > 0 ? head.y : FIELD_HEIGHT) - 1;
            break;
        case DOWN:
            head.y = (head.y + 1) % FIELD_HEIGHT;
            break;
        case LEFT:
            head.x = (head.x > 0 ? head.x : FIELD_WIDTH) - 1;
            break;
        case RIGHT:
            head.x = (head.x + 1) % FIELD_WIDTH;
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

void run_game()
{
    Game *game = game_init();
    new_food(game);

    puts("starting");
    int playing = 1;
    int paused = 0;

    while (playing) {
        draw_field(game->field);
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
                playing = 0;
                break;
        }

        playing = update_snake(game);
        SDL_Delay(100);
    }

    handle_score(game->score);
    free_game(game);
}

