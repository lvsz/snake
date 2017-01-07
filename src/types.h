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
    char **field;
    size_t width;
    size_t height;
    Snake *snake;
    int score;
} Game;

typedef struct HighScore {
    int score;
    char name[4];
} HighScore;

typedef enum Input {
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
