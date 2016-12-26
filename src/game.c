#include "game.h"

Game *game_init()
{
    Game *game = malloc(sizeof(Game));
    game->field = malloc(FIELD_WIDTH * sizeof(char *));
    for (size_t i = 0; i < FIELD_WIDTH; ++i) {
        (game->field)[i] = calloc(FIELD_HEIGHT, sizeof(char));
    }
    return game;
}

void free_game(Game *game)
{
    printf("freeing %i subfields\n", FIELD_WIDTH);
    for (size_t i = 0; i < FIELD_WIDTH; ++i) {
        free((game->field)[i]);
    }
    puts("freeing field");
    free(game->field);
    puts("freeing snake");
    free_snake(game->snake);
    puts("freeing game");
    free(game);
}

void place_snake(Game *game, size_t start_x, size_t start_y, Direction direction)
{
    puts("placing snake");
    if (direction != LEFT && direction != RIGHT) {
        fprintf(stderr, "Invalid intial direction, defaulting to RIGHT\n");
        direction = RIGHT;
    }
    game->snake = new_snake(NULL, 0, direction);
    for (int i = SNAKE_SIZE - 1; i >= 0; --i) {
        size_t x = direction == LEFT ? start_x + i : start_x - i + 1;
        Point p = {x, start_y};
        add_head(game->snake, p);
        game->field[x][start_y] = 's';
    }
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
    printf("is_food? x: %lu, y: %lu\n", p.x, p.y);
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

void update_snake(Game *game)
{
    Point head = snake_head(game->snake);
    printf("old head: (%lu, %lu)\n", head.x, head.y);
    switch (game->snake->direction) {
        case UP:
            puts("up");
            head.y = (head.y > 0 ? head.y : FIELD_HEIGHT) - 1;
            break;
        case DOWN:
            puts("down");
            head.y = (head.y + 1) % FIELD_HEIGHT;
            break;
        case LEFT:
            puts("left");
            head.x = (head.x > 0 ? head.x : FIELD_WIDTH) - 1;
            break;
        case RIGHT:
            puts("right");
            head.x = (head.x + 1) % FIELD_WIDTH;
            break;
    }
    printf("new head: (%lu, %lu)\n", head.x, head.y);
    if (is_food(game, head)) {
        add_head(game->snake, head);
        game->field[head.x][head.y] = 's';
        new_food(game);
    } else {
        Point tail = pop_tail(game->snake);
        printf("tail.x: %lu, tail.y: %lu\n", tail.x, tail.y);
        game->field[tail.x][tail.y] = '\0';
        puts("popped tail");
        switch (game->field[head.x][head.y]) {
            case 's':
                puts("game over");
                pause();
                break;
            default:
                add_head(game->snake, head);
                game->field[head.x][head.y] = 's';
                break;
        }
    }
}

void run_game()
{
    Game *game = game_init();
    place_snake(game, SNAKE_START_X, SNAKE_START_Y, RIGHT);
    new_food(game);
    print_field(game->field);

    puts("starting");
    int playing = 1;
    int paused = 0;
    while (playing) {
        draw_field(game->field);
        update_snake(game);
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
        SDL_Delay(100);
    }
    puts("free game");
    free_game(game);
}

