#include "snake.h"

#define BUFFER (FIELD_WIDTH * FIELD_HEIGHT)

Snake *new_snake(Point *argv, size_t argc, Direction direction)
{
    Snake *snake = malloc(sizeof(Snake));
    if (snake == NULL) {
        fprintf(stderr, "Error: allocation of snake failed");
        exit(1);
    }
    puts("allocating body");
    snake->body = malloc(BUFFER * sizeof(Point));
    if (snake == NULL) {
        fprintf(stderr, "Error: allocation of snake body failed");
        exit(1);
    }
    printf("### body ptr: %u\n", (unsigned) snake->body);
    printf("assigning size: %lu\n", argc);
    snake->size = argc;
    puts("assigning direction");
    snake->direction = direction;
    puts("assigning head");
    snake->head = &(snake->body[BUFFER - 1]);
    puts("assigning tail");
    snake->tail = snake->body;
    puts("assigning EOB");
    snake->_end_of_buffer = &(snake->body[BUFFER - 1]);

    for (int i = 0; i < argc; ++i) {
        add_head(snake, argv[i]);
    }
    return snake;
}

void free_snake(Snake *snake)
{
    free(snake->body);
    free(snake);
}

Point snake_head(Snake *snake)
{
    if (snake->size != 0) {
        return *(snake->head);
    } else {
        fprintf(stderr, "Error: calling head() on empty snake");
        exit(1);
    }
}


void add_head(Snake *snake, Point p)
{
    if (snake->head == snake->_end_of_buffer) {
        snake->head = snake->body;
    } else {
        ++(snake->head);
    }
    snake->head->x = p.x;
    snake->head->y = p.y;
    ++(snake->size);
}

Point pop_tail(Snake *snake)
{
    if ((snake->size)-- == 0) {
        fprintf(stderr, "Error: calling pop_tail() on empty snake");
        exit(1);
    } else if (snake->tail == snake->_end_of_buffer) {
        snake->tail = snake->body;
        return snake->body[BUFFER - 1];
    } else {
        return *((snake->tail)++);
    }
}

void print_snake(FILE * restrict stream, Snake *snake)
{
    Point *ptr = snake->tail;
    for (int i = 0; i < snake->size; ++i) {
        fprintf(stream, "%lu %lu\n", ptr->x, ptr->y);
        ptr = ptr == snake->_end_of_buffer ? snake->body : ptr + 1;
    }
}
