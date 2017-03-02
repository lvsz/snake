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
    /* eob = end of buffer */
    snake->eob = &(snake->body[SNAKE_BUFFER - 1]);

    return snake;
}

/* places snake on the game's field
 * player can be 1 or 2 */
void place_snake(Game *game, int player, Point p, Direction direction)
{
    if (direction != LEFT && direction != RIGHT) {
        /* allowing vertical placement would make it more complex
         * and I don't have any use for it at the moment */
        fprintf(stderr, "Invalid intial direction, defaulting to RIGHT\n");
        direction = RIGHT;
    }

    /* a pointer to a snake pointer to avoid segfaults when uninitialised */
    Snake **snake = player == 1 ? &(game->p1) : &(game->p2);
    *snake = new_snake(player, direction);

    for (int i = SNAKE_SIZE - 1; i >= 0; --i) {
        size_t x = direction == LEFT ? p.x + i : p.x - i + 1;
        Point tmp = {x, p.y};
        add_head(*snake, tmp);
        game->field[x][p.y] = (*snake)->player;
    }
}

/* remove the snake from the field */
void clear_snake(Game *game, Snake *snake)
{
    while (snake->tail != snake->head) {
        Point tail = pop_tail(snake);
        game->field[tail.x][tail.y] = '\0';
    }

    game->field[snake->head->x][snake->head->y] = '\0';
    free(snake);
}

/* moves the snake and checks for food
 * the new "head" won't be added to the field until it checks for obstacles */
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
        food_sound();
        add_head(snake, head);
        snake->score += 1;
        new_food(game, FOOD);
    } else if (is_treat(game, head)) {
        treat_sound();
        add_head(snake, head);
        snake->score += TREAT_POINTS;
        remove_treat(game);
    } else {
        Point tail = pop_tail(snake);
        game->field[tail.x][tail.y] = '\0';
        add_head(snake, head);
    }
}

/* checks if the snake hit a wall, itself, or another snake
 * if not, add the new head to the field so it can be drawn */
int check_snake (Game *game, Snake *snake)
{
    Point head = snake_head(snake);
    switch (game->field[head.x][head.y]) {
        case '1':
        case '2':
        case 'w':
            puts("game over");
            death_sound();
            if (game->players > 1)
                snake->score = -1;
            return 0;
        default:
            game->field[head.x][head.y] = snake->player;
            return 1;
    }
}

/* get the pointer to the snake's head
 * exits when there is not head */
Point snake_head(Snake *snake)
{
    if (snake->head == snake->tail) {
        fprintf(stderr, "Error: calling head() on empty snake\n");
        exit(1);
    } else {
        return *(snake->head);
    }
}

/* add a new head to the snake at point P */
void add_head(Snake *snake, Point p)
{
    if (snake->head == snake->eob) {
        /* if at end of buffer, add it to the start of the buffer */
        snake->head = snake->body;
    } else {
        ++(snake->head);
    }

    snake->head->x = p.x;
    snake->head->y = p.y;
}

/* returns pointer to the snake's tail and removes it */
Point pop_tail(Snake *snake)
{
    if (snake->head == snake->tail) {
        fprintf(stderr, "Error: calling pop_tail() on empty snake\n");
        exit(1);
    } else if (snake->tail == snake->eob) {
        snake->tail = snake->body;
        return *(snake->eob);
    } else {
        return *((snake->tail)++);
    }
}

