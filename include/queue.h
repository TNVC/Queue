#ifndef QUEUE_H_
#define QUEUE_H_

#include <stddef.h>
#include "settings.h"

struct Queue {
  element_t *data;
  size_t capacity;
  size_t size;
  size_t head;
  size_t tail;
};

void initQueue(Queue *queue, size_t capacity);

void destroyQueue(Queue *queue);

void queue_push(Queue *queue, element_t el);

element_t queue_pop(Queue *queue);

size_t queue_size(const Queue *queue);

size_t queue_capacity(const Queue *queue);

void dumpQueue(const Queue *queue);

#endif
