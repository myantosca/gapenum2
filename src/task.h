#ifndef __TASK_H
#define __TASK_H

#include <stdlib.h>
#include <sys/types.h>

typedef int64_t task_time_t;

typedef struct task
{
  task_time_t r; // task release
  task_time_t c; // computation time
  task_time_t p; // task period
} task_t;

#endif
