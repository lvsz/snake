#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Queue_node {
    void *value;
    struct Queue_node *next;
} Queue_node;

typedef struct Queue {
    size_t width;
    Queue_node *front;
    Queue_node *rear;
} Queue;

Queue *new_queue(size_t width);
void free_queue(Queue *);
int is_empty(Queue *);

void next_in_queue(Queue *q, void *);
void last_in_queue(Queue *q, void *);

void enqueue(Queue *, void *);
void dequeue(Queue *, void *);

void for_each_in_queue(void (*fn)(void *), Queue *);

#endif /* _QUEUE_H_ */

