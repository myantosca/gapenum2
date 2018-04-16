#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

     printf("n,j,RT_j,steps,μs,r1,c1,p1,r2,c2,p2,r3,c3,p3,r4,c4,p4,r5,c5,p5,r6,c6,p6,r7,c7,p7,r8,c8,p8\n");
     int j;
     for (j = n-1; j >= 0; j--)
     {
	  size_t comp_steps = 0;
	  struct timespec t_0 = {0,0};
	  struct timespec t_f = {0,0};
	  clock_gettime(CLOCK_MONOTONIC, &t_0);
	  rt_j = gap_enum(tasks, n, j, 1, &comp_steps);
	  clock_gettime(CLOCK_MONOTONIC, &t_f);
	  size_t t_exec = (t_f.tv_sec - t_0.tv_sec) * 1000000 + (size_t)((t_f.tv_nsec - t_0.tv_nsec) * .001);
	  printf("%lu,%d,%ld,%lu,%lu,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n",
		 n, j+1, rt_j, comp_steps, t_exec,
		 tasks[0].r, tasks[0].c, tasks[0].p,
		 tasks[1].r, tasks[1].c, tasks[1].p,
		 tasks[2].r, tasks[2].c, tasks[2].p,
		 tasks[3].r, tasks[3].c, tasks[3].p,
		 tasks[4].r, tasks[4].c, tasks[4].p,
		 tasks[5].r, tasks[5].c, tasks[5].p,
		 tasks[6].r, tasks[6].c, tasks[6].p,
		 tasks[7].r, tasks[7].c, tasks[7].p);
	  if (rt_j < 0) break;
     }
     return (rt_j < 0);
}
