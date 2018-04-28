#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "gap_enum.h"
#include "gap_enum2.h"

rb_time_t exec_gap_enum(int version, task_t* tasks, size_t m, size_t n, int j)
{
     rb_time_t rt_j = -1;
     size_t comp_steps = 0;
     size_t gap_ct = 0;
     struct timespec t_0 = {0,0};
     struct timespec t_f = {0,0};
     clock_gettime(CLOCK_MONOTONIC, &t_0);
     if (version == 1)
     {
	  rt_j = gap_enum(tasks, n, j, 1, &comp_steps, &gap_ct);
     }
     else if (version == 2)
     {
	  rt_j = gap_enum2(tasks, n, j, 1, &comp_steps, &gap_ct);
     }
     clock_gettime(CLOCK_MONOTONIC, &t_f);
     size_t t_exec = (t_f.tv_sec - t_0.tv_sec) * 1000000 + (size_t)((t_f.tv_nsec - t_0.tv_nsec) * .001);
     printf("gap_enum%d,%lu,%lu,%d,%ld,%lu,%lu,%lu", version, m, n, j+1, rt_j, gap_ct, comp_steps, t_exec);
     int jj;
     for (jj = 0; jj < n; jj++)
     {
	  printf(",%ld,%ld,%ld", tasks[jj].r, tasks[jj].c, tasks[jj].p);
     }
     printf("\n");
     return rt_j;
}

int main(int argc, char *argv[])
{
     int err = 0;
     size_t m = 1;
     printf("m,n,j,RT_j,gaps,steps,μs,r1,c1,p1,r2,c2,p2,r3,c3,p3,r4,c4,p4,r5,c5,p5,r6,c6,p6,r7,c7,p7,r8,c8,p8\n");
     while (!feof(stdin))
     {
	  size_t n = 0;
	  int j;
	  scanf("%lu:", &n);
	  task_t *tasks = malloc(n * sizeof(task_t));
	  memset(tasks, 0, sizeof(task_t) * n);
	  for (j = 0; j < n; j++)
	  {
	       scanf("{%ld,%ld,%ld}", &(tasks[j].r), &(tasks[j].c), &(tasks[j].p));
	  }

	  rb_time_t rt_j = -1;
	  for (j = n-1; j >= 0; j--)
	  {
	       rt_j = exec_gap_enum(1, tasks, m, n, j);
	       rt_j = exec_gap_enum(2, tasks, m, n, j);
	       if (rt_j < 0) break;
	  }
	  err |= (rt_j < 0);
	  if (tasks) free(tasks);
	  m++;
     }
     return err;
}
