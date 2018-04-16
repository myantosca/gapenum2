#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "gap_enum.h"


int main(int argc, char *argv[])
{
     task_t tasks[8];
     memset(tasks, 0, sizeof(task_t) * 8);
     size_t n = 0;
     int done = 0;
     rb_time_t rt_j = -1;
     while (scanf("{%ld,%ld,%ld}\n", &(tasks[n].r), &(tasks[n].c), &(tasks[n].p)) == 3) n++;

     int j;
     for (j = n-1; j >= 0; j--)
     {
	  size_t comp_steps = 0;
	  struct timespec t_0 = {0,0};
	  struct timespec t_f = {0,0};
	  clock_gettime(CLOCK_MONOTONIC, &t_0);
	  rt_j = gap_enum(tasks, n, j, 1, &comp_steps);
	  clock_gettime(CLOCK_MONOTONIC, &t_f);
	  printf("RT(τ[%d] = {r=%ld,c=%ld,p=%ld}) = %ld [%lu/%lu]\n",
		 j, tasks[j].r, tasks[j].c, tasks[j].p, rt_j, comp_steps, (t_f.tv_sec - t_0.tv_sec) * 1000000 + (ulong)((t_f.tv_nsec - t_0.tv_nsec) * .001));
	  if (rt_j < 0) break;
     }
     return (rt_j < 0);
}
