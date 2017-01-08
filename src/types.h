#ifndef _TYPES_H_
#define _TYPES_H_

#include "defaults.h"

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
    Point body[SNAKE_BUFFER];
    Point *head;
    Point *tail;
    Point *eob;
    Direction direction;
} Snake;

typedef enum Food {
    FOOD,
    TREAT
} Food;

typedef struct Game {
    char **field;
    size_t width;
    size_t height;
    Snake *p1;
    int score;
    int turns;
    char *treat;
} Game;

typedef struct HighScore {
    int score;
    char name[4];
} HighScore;

typedef enum Input {
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    QUIT,
    P1_UP,
    P1_DOWN,
    P1_LEFT,
    P1_RIGHT,
    PAUSE,
    SAVE,
    LOAD,
    NOTHING
} Input;

#endif /* _TYPES_H_ */
