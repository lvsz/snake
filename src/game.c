#include "game.h"

static char **field;

void create_field()
{
    field = malloc(FIELD_WIDTH * sizeof(char *));
    for (size_t i = 0; i < FIELD_WIDTH; ++i) {
        field[i] = calloc(FIELD_HEIGHT, sizeof(char));
    }
}

void destroy_field()
{
    for (size_t i = 0; i < FIELD_WIDTH; ++i) {
        free(field[i]);
    }
    free(field);
}

Snake *place_snake(size_t start_x, size_t start_y, Direction direction)
{
    puts("placing snake");
    if (direction != LEFT && direction != RIGHT) {
        fprintf(stderr, "Invalid intial direction, defaulting to RIGHT\n");
        direction = RIGHT;
    }
    Snake *snake = new_snake();
    for (int i = SNAKE_SIZE - 1; i >= 0; --i) {
        size_t x = direction == LEFT ? start_x + i : start_x - i + 1;
        Point p = {x, start_y};
        add_head(snake, p);
        field[x][start_y] = 's';
    }
    return snake;
}

void pause()
{
    puts("pause");
    while (read_input() != PAUSE) {
        SDL_Delay(100);
    }
}

static FILE *savefile;
void print_point(void *v)
{
    Point *p = (Point *) v;
    fprintf(savefile, "|%d %d|\n", p->x, p->y);
}

void save(Snake *snake)
{
    savefile = fopen("save.txt", "w");
    for_each_in_queue(print_point, snake);
    fclose(savefile);
}

int is_food(Point p)
{
    return field[p.x][p.y] == 'f';
}

void new_food()
{
    srand(time(NULL));
    size_t x = rand() % FIELD_WIDTH;
    size_t y = rand() % FIELD_HEIGHT;
    while (field[x][y] != '\0') {
        x = rand() % FIELD_WIDTH;
        y = rand() % FIELD_HEIGHT;
    }
    field[x][y] = 'f';
}

void update_snake(Snake *snake, Direction direction)
{
    Point head = head_point(snake);
    switch (direction) {
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
    if (is_food(head)) {
        add_head(snake, head);
        field[head.x][head.y] = 's';
        new_food();
    } else {
        Point tail = pop_tail(snake);
        field[tail.x][tail.y] = '\0';
        switch (field[head.x][head.y]) {
            case 's':
                pause();
                break;
            default:
                add_head(snake, head);
                field[head.x][head.y] = 's';
                break;
        }
    }
}

void run_game()
{
    create_field();
    Direction direction = RIGHT;
    Snake *snake = place_snake(SNAKE_START_X, SNAKE_START_Y, direction);
    new_food();

    puts("starting");
    int playing = 1;
    int paused = 0;
    while (playing) {
        draw_field(field);
        switch (read_input()) {
            case NOTHING:
                break;
            case P1_UP:
                if (direction != DOWN)
                    direction = UP;
                break;
            case P1_DOWN:
                if (direction != UP)
                    direction = DOWN;
                break;
            case P1_LEFT:
                if (direction != RIGHT)
                    direction = LEFT;
                break;
            case P1_RIGHT:
                if (direction != LEFT)
                    direction = RIGHT;
                break;
            case PAUSE:
                paused ^= 1;
                pause();
                break;
            case SAVE:
                save(snake);
                break;
            case QUIT:
                playing = 0;
                break;
        }
        update_snake(snake, direction);
        SDL_Delay(100);
    }
    destroy_field();
}

