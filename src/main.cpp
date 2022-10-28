#include <stdio.h>
#include "function.h"
#include "queue.h"

int main()
{
  Queue queue = {};

  initQueue(&queue, 80);

  init(&queue);

  while (wait())
    continue;

  return 0;
}
