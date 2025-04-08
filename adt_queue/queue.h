#ifndef QUEUE_H_
#define QUEUE_H_

typedef int boolean;
#define TRUE 1
#define FALSE 0

typedef struct queue_str queue;

queue *queue_create();
void queue_destroy(queue *);

int queue_size(queue *);
boolean queue_isEmpty(queue *);

void queue_enqueue(queue *, void *);
void *queue_dequeue(queue *);

void *queue_front(queue *);

#endif