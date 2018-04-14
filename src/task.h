#ifndef __TASK_H
#define __TASK_H

#include <stdlib.h>

typedef struct task
{
  int r; // task release
  int c; // computation time
  int p; // task period
} task_t;

#endif
