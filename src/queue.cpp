#include "queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void initQueue(Queue *queue, size_t capacity)
{
  queue->capacity = capacity ? capacity + 1 : 0;
  queue->size     = 0;
  queue->head     = 0;
  queue->tail     = 0;

  if (capacity)
    queue->data = (element_t *)calloc(capacity + 1, sizeof(element_t));
}

void destroyQueue(Queue *queue)
{
  queue->capacity = 0;
  queue->size     = 0;
  queue->head     = 0;
  queue->tail     = 0;

  if (queue->capacity)
    free(queue->data);
}

void queue_push(Queue *queue, element_t el)
{
  if (queue->size == queue->capacity)
    return;

  queue->data[queue->tail] = el;

  queue->tail = (queue->tail + 1) % queue->capacity;

  ++queue->size;
}

element_t queue_pop(Queue *queue)
{
  if (!queue->size)
    return 0;

  --queue->size;

  element_t el = queue->data[queue->head];

  queue->data[queue->head] = 0;

  queue->head = (queue->head + 1) % queue->capacity;

  return el;
}

size_t queue_size(const Queue *queue)
{
  return queue->size;
}

size_t queue_capacity(const Queue *queue)
{
  return queue->capacity;
}

void dumpQueue(const Queue *queue)
{
  for (size_t i = 0; i < queue->capacity / 32 + 1; ++i)
    {
      printf("\r");

      size_t max = (queue->capacity - i*32 < 32) ? queue->capacity - i*32 : 32;

      for (size_t j = 0; j < max; ++j)
        printf("----");
      printf("-\r\n");

      for (size_t j = 0; j < max; ++j)
        {
          size_t val = i*32 + j;

          int between =
            ((queue->tail < queue->head) && (val < queue->tail || val > queue->head)) ||
            ((queue->tail >= queue->head) && (val < queue->tail && val > queue->head));

          if (val == queue->head)
            printf("|\033[41m%3zu\033[0m", val);
          else if (val == queue->tail)
            printf("|\033[44m%3zu\033[0m", val);
          else if (between)
            printf("|\033[42m%3zu\033[0m", val);
          else
            printf("|%3zu", val);
        }

      printf("|\r\n");

      for (size_t j = 0; j < max; ++j)
        printf("----");
      printf("-\r\n");

      for (size_t j = 0; j < max; ++j)
        {
          size_t val = i*32 + j;

          char ch = isgraph(queue->data[i*32 + j]) ?
            queue->data[i*32 + j] :
            '~';

          int between =
            ((queue->tail < queue->head) && (val < queue->tail || val > queue->head)) ||
            ((queue->tail >= queue->head) && (val < queue->tail && val > queue->head));

          if (val == queue->head)
            printf("|\033[41m %c \033[0m", ch);
          else if (val == queue->tail)
            printf("|\033[44m %c \033[0m", ch);
          else if (between)
            printf("|\033[42m %c \033[0m", ch);
          else
            printf("| %c ", ch);
        }

      printf("|\r\n");

      for (size_t j = 0; j < max; ++j)
        printf("----");
      printf("-\r\n");
    }
}
