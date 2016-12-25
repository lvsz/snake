#include "snake.h"

Snake *new_snake(Point *argv, int argc)
{
    Snake *snake = new_queue(sizeof(Point));
    return snake;
}

void free_snake(Snake *snake)
{
    free_queue(snake);
}

Point head_point(Snake *snake)
{
    Point p;
    last_in_queue(snake, &p);
    return p;
}

void add_head(Snake *snake, Point p)
{
    enqueue(snake, &p);
}

Point pop_tail(Snake *snake)
{
    Point p;
    dequeue(snake, &p);
    return p;
}

