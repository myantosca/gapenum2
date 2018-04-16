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

     int j;
     for (j = n-1; j >= 0; j--)
     {
	  size_t comp_steps = 0;
	  rt_j = gap_enum(tasks, n, j, 1, &comp_steps);
	  printf("RT(τ[%d] = {r=%ld,c=%ld,p=%ld}) = %ld [%lu]\n",
		 j, tasks[j].r, tasks[j].c, tasks[j].p, rt_j, comp_steps);
	  if (rt_j < 0) break;
     }
     return (rt_j < 0);
}
