#include "snake.h"

Snake *new_snake(Direction direction)
{
    Snake *snake = malloc(sizeof(Snake));
    if (snake == NULL) {
        fprintf(stderr, "Error: allocation of snake failed\n");
        exit(1);
    }

    puts("assigning direction");
    snake->direction = direction;
    puts("assigning head");
    snake->head = &(snake->body[SNAKE_BUFFER - 1]);
    puts("assigning tail");
    snake->tail = snake->body;
    puts("assigning EOB");
    snake->eob = &(snake->body[SNAKE_BUFFER - 1]);

    return snake;
}

void free_snake(Snake *snake)
{
    puts("freeing snake");
    free(snake);
}

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
        puts("eating food");
        add_head(game->snake, head);
        game->field[head.x][head.y] = 's';
        game->score++;
        new_food(game, FOOD);
        return 1;
    } else if (is_treat(game, head)) {
        puts("eating treat");
        add_head(game->snake, head);
        game->field[head.x][head.y] = 's';
        game->score += TREAT_POINTS;
        remove_treat(game);
        return 1;
    } else {
        Point tail = pop_tail(game->snake);
        game->field[tail.x][tail.y] = '\0';

        switch (game->field[head.x][head.y]) {
            case 's':
            case 'w':
                puts("game over");
                return 0;
            default:
                add_head(game->snake, head);
                game->field[head.x][head.y] = 's';
                return 1;
        }
    }
}

Point snake_head(Snake *snake)
{
    if (snake->head == snake->tail) {
        fprintf(stderr, "Error: calling head() on empty snake\n");
        exit(1);
    } else {
        return *(snake->head);
    }
}


void add_head(Snake *snake, Point p)
{
    if (snake->head == snake->eob) {
        snake->head = snake->body;
    } else {
        ++(snake->head);
    }
    snake->head->x = p.x;
    snake->head->y = p.y;
}

Point pop_tail(Snake *snake)
{
    if (snake->head == snake->tail) {
        fprintf(stderr, "Error: calling pop_tail() on empty snake\n");
        exit(1);
    } else if (snake->tail == snake->eob) {
        snake->tail = snake->body;
        return snake->body[SNAKE_BUFFER - 1];
    } else {
        return *((snake->tail)++);
    }
}

void print_snake(FILE *stream, Snake *snake)
{
    fprintf(stream, "%d;", snake->direction);
    fprintf(stream, "%d;", (int) (snake->head - snake->body));
    fprintf(stream, "%d;", (int) (snake->tail - snake->body));
    fprintf(stdout, "%d;", (int) (snake->head - snake->body));
    fprintf(stdout, "%d;", (int) (snake->tail - snake->body));
    fwrite(snake->body, sizeof(Point), SNAKE_BUFFER, stream);
}

