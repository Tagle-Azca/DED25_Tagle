#include "queue.h"
#include <stdlib.h>

struct node_str
{
    void *data;
    struct node_str *next;
};

typedef struct node_str node;

struct queue_str
{
    node *front;
    node *back;
    int size;
};

queue *queue_create()
{
    queue *q = malloc(sizeof(struct queue_str));
    q->front = NULL;
    q->back = NULL;
    q->size = 0;
    return q;
}

void queue_destroy(queue *q)
{
    node *n = q->front;
    while (n != NULL)
    {
        node *next = n->next;
        free(n);
        n = next;
    }
    free(q);
}

boolean queue_isEmpty(queue *q)
{
    return q->front == NULL ? TRUE : FALSE;
}

int queue_size(queue *q)
{
    return q->size;
}

void queue_enqueue(queue *q, void *data)
{
    node *n = malloc(sizeof(struct node_str));
    n->data = data;
    n->next = NULL;

    if (q->back == NULL)
    {
        q->front = n;
        q->back = n;
    }
    else
    {
        q->back->next = n;
        q->back = n;
    }
    q->size++;
}

void *queue_dequeue(queue *q)
{
    if (q->front == NULL)
        return NULL;

    void *data = q->front->data;
    node *temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL)
        q->back = NULL;

    free(temp);
    q->size--;
    return data;
}

void *queue_front(queue *q)
{
    if (q->front == NULL)
        return NULL;
    return q->front->data;
}