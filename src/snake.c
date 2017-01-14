#include "snake.h"

Snake *new_snake(int player, Direction direction)
{
    Snake *snake = malloc(sizeof(Snake));
    if (snake == NULL) {
        fprintf(stderr, "Error: allocation of snake failed\n");
        exit(1);
    }

    snake->player = player + '0';
    snake->score = 0;
    snake->direction = direction;
    snake->head = &(snake->body[SNAKE_BUFFER - 1]);
    snake->tail = snake->body;
    snake->eob = &(snake->body[SNAKE_BUFFER - 1]);

    return snake;
}

void place_snake(Game *game, int player, Point p, Direction direction)
{
    if (direction != LEFT && direction != RIGHT) {
        fprintf(stderr, "Invalid intial direction, defaulting to RIGHT\n");
        direction = RIGHT;
    }

    Snake **snake = player == 1 ? &(game->p1) : &(game->p2);
    *snake = new_snake(player, direction);

    for (int i = SNAKE_SIZE - 1; i >= 0; --i) {
        size_t x = direction == LEFT ? p.x + i : p.x - i + 1;
        Point tmp = {x, p.y};
        add_head(*snake, tmp);
        game->field[x][p.y] = (*snake)->player;
    }
}

void clear_snake(Game *game, Snake *snake)
{
    while (snake->tail != snake->head) {
        Point tail = pop_tail(snake);
        game->field[tail.x][tail.y] = '\0';
    }

    game->field[snake->head->x][snake->head->y] = '\0';
    free(snake);
}

void move_snake(Game *game, Snake *snake)
{
    Point head = snake_head(snake);

    switch (snake->direction) {
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
        add_head(snake, head);
        snake->score += 1;
        new_food(game, FOOD);
    } else if (is_treat(game, head)) {
        puts("eating treat");
        add_head(snake, head);
        snake->score += TREAT_POINTS;
        remove_treat(game);
    } else {
        Point tail = pop_tail(snake);
        game->field[tail.x][tail.y] = '\0';
        add_head(snake, head);
    }
}

int check_snake (Game *game, Snake *snake)
{
    Point head = snake_head(snake);
    switch (game->field[head.x][head.y]) {
        case '1':
        case '2':
        case 'w':
            puts("game over");
            if (game->players > 1)
                snake->score = -1;
            return 0;
        default:
            game->field[head.x][head.y] = snake->player;
            return 1;
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

