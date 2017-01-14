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
    char player;
    Point body[SNAKE_BUFFER];
    Point *head;
    Point *tail;
    Point *eob;
    int score;
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
    int players;
    Snake *p1;
    Snake *p2;
    int level;
    int total_score;
    int turns;
    int speed;
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
    P2_UP,
    P2_DOWN,
    P2_LEFT,
    P2_RIGHT,
    PAUSE,
    SAVE,
    LOAD,
    MULTIPLAYER,
    NOTHING
} Input;

#endif /* _TYPES_H_ */
