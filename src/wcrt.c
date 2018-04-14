#include <stdlib.h>
#include <stdio.h>

#include "gap_enum.h"


int main(int argc, char *argv[])
{
     task_t tasks[32];
     size_t n = 0;
     int done = 0;
     rb_time_t rt_j = -1;

     while (scanf("{%ld,%ld,%ld}\n", &(tasks[n].r), &(tasks[n].c), &(tasks[n].p)) == 3) n++;

     size_t j;
     for (j = n-1; j >= 0; j--)
     {
	  rt_j = gap_enum(tasks, n, j, 1);
	  printf("RT(τ[%ld] = {r=%ld,c=%ld,p=%ld}) = %ld\n",
		 j, tasks[j].r, tasks[j].c, tasks[j].p, rt_j);
	  if (rt_j < 0) break;
     }
     return (rt_j < 0);
}
