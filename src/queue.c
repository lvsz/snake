#include "queue.h"

Queue *new_queue(size_t width)
{
    Queue *q = malloc(sizeof(Queue));
    if (q != NULL) {
        q->width = width;
        q->front = NULL;
        q->rear = NULL;
        return q;
    }
    fprintf(stderr, "Error: queue allocation failed\n");
    exit(1);
}

void free_queue(Queue *q)
{
    Queue_node *tmp;
    while (q->front != NULL) {
        tmp = q->front;
        q->front = tmp->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

int is_empty(Queue *q)
{
    return q->front == NULL;
}

void next_in_queue(Queue *q, void *res)
{
    if (!is_empty(q)) {
        memcpy(res, q->front->value, q->width);
    } else {
        fprintf(stderr, "Error: accessing empty queue\n");
        exit(1);
    }
}

void last_in_queue(Queue *q, void *res)
{
    if (!is_empty(q)) {
        memcpy(res, q->rear->value, q->width);
    } else {
        fprintf(stderr, "Error: accessing empty queue\n");
        exit(1);
    }
}

void enqueue(Queue *q, void *val)
{
    if (q->rear == NULL) {
        q->rear = malloc(sizeof(Queue_node));
        q->rear->value = malloc(q->width);
        q->rear->next = NULL;
        memcpy(q->rear->value, val, q->width);
        q->front = q->rear;
    } else {
        q->rear->next = malloc(sizeof(Queue_node));
        q->rear->next->value = malloc(q->width);
        q->rear->next->next = NULL;
        memcpy(q->rear->next->value, val, q->width);
        q->rear = q->rear->next;
    }
}

void dequeue(Queue *q, void *res)
{
    if (!is_empty(q)) {
        Queue_node *tmp = q->front;
        memcpy(res, tmp->value, q->width);
        q->front = tmp->next;
        free(tmp->value);
        free(tmp);
    } else {
        fprintf(stderr, "Error: dequeuing empty queue\n");
        exit(1);
    }
}

void for_each_in_queue(void (*fn)(void *), Queue *q)
{
    Queue_node *x = q->front;
    while (x->next != NULL) {
        fn(x->value);
        x = x->next;
    }
}

