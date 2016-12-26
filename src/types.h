#ifndef _TYPES_H_
#define _TYPES_H_

typedef struct Point {
    size_t x;
    size_t y;
} Point;

typedef enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct Snake {
    Point *body;
    Point *head;
    Point *tail;
    Point *_end_of_buffer;
    Direction direction;
} Snake;

typedef struct Game {
    Snake *snake;
    char **field;
} Game;


#endif /* _TYPES_H_ */
