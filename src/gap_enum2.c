#include <math.h>
#include <string.h>

#include "gap_list.h"
#include "gap_enum2.h"

gap_list_t *gap_xfrm2(gap_t W, gap_list_t *gaps, task_t *tasks, int j, size_t *comp_steps)
{
     // Get the number of jobs for the task that will occur within W.
     size_t jobs = ceil(((double)W.exit - (double)tasks[j].r)/(double)tasks[j].p);
     size_t q;
     char buf[8192];
     for (q = 1; q <= jobs; q++)
     {
	  // Initialize the time counter to the current job's release time.
	  task_time_t t = tasks[j].r + (q-1) * tasks[j].p;
	  (*comp_steps)++;
	  gap_list_node_t *k_gap = gaps->head;
	  while (k_gap != gaps->tail)
	  {
	       (*comp_steps)++;
	       task_time_t t1 = k_gap->gap.entry;
	       task_time_t t2 = k_gap->gap.exit;
	       gap_list_node_t *free_gap = NULL;
	       // No gap exists that can accommodate the current job. Bail.
	       if (t1 > t + tasks[j].p) return NULL;

	       // Bump effective release time to current gap under examination.
	       if (t < t1) t = t1;
	       // Check if effective release time falls w/in the current gap.
	       if ((t1 <= t) && (t < t2))
	       {
		    // Job fits with slack at the start or does not fit.
		    if (t + tasks[j].c >= t2)
		    {
			 if (t > t1)
			 {
			      k_gap->gap.exit = t;
			 }
			 else
			 {
			      free_gap = k_gap;
			      k_gap = k_gap->next;
			      free_gap = gl_delete(gaps, free_gap, comp_steps);
			      if (free_gap) free(free_gap);
			      free_gap = NULL;
			 }

			 // Break out of loop if not an abort scenario.
			 if (t + tasks[j].c == t2) break;
		    }
		    // Job fits with slack on either side.
		    if (t + tasks[j].c < t2)
		    {
			 k_gap->gap.entry = t + tasks[j].c;
			 if (t > t1) gl_insert(gaps, k_gap, (gap_t){t1, t}, comp_steps);
			 break;
		    }

	       }

	       if (k_gap->gap.entry == t1)
	       {
		    k_gap = k_gap->next;
	       }
	  }
     }

     // Return the modified gap tree.
     return gaps;
}

gap_t gap_srch2(gap_list_t *gaps, task_time_t c, size_t *comp_steps)
{
     gap_t gap = { -1, 0 };
     if (gaps)
     {
	  gap_list_node_t* X = gaps->head;
	  while (X != gaps->tail && c > X->gap.exit - X->gap.entry)
	  {
	       (*comp_steps)++;
	       X = X->next;
	  }
	  gap = X->gap;
     }
     return gap;
}

task_time_t gap_enum2(task_t *tasks, size_t n, size_t j, int windows, size_t *comp_steps, size_t *gap_ct)
{
     task_time_t w = (task_time_t)ceil((double)tasks[j].p/(double)windows);
     task_time_t L = w;
     task_time_t U = tasks[j].p + w;
     char buf[8192];
     while (L < U)
     {
	  gap_list_t *gaps = alloc_gap_list();
	  gl_insert(gaps, gaps->head, (gap_t){ 0, L }, comp_steps);
	  int i;
	  for (i = n-1; i > j; i--)
	  {
	       (*comp_steps)++;
	       gap_list_t *gaps2 = gap_xfrm2((gap_t){ 0, L }, gaps, tasks, i, comp_steps);
	       if (!gaps2)
	       {
		    *gap_ct = gl_size(gaps);
		    memset(buf, 0, 8192);
		    sprintf_gap_list(buf, gaps);
		    fprintf(stderr, "%d: %s\n", i, buf);
		    free_gap_list(gaps);
		    return -1;
	       }
	  }
	  gap_t gap = gap_srch2(gaps, tasks[j].c, comp_steps);
	  task_time_t rt_j = -1;
	  if (gap.entry >= 0) rt_j = gap.entry + tasks[j].c;
	  if (rt_j < tasks[j].p)
	  {
	       *gap_ct = gl_size(gaps);
	       memset(buf, 0, 8192);
	       sprintf_gap_list(buf, gaps);
	       fprintf(stderr, "%d: %s\n", i, buf);
	       free_gap_list(gaps);
	       return rt_j;
	  }
	  free_gap_list(gaps);
	  L += w;
     }
     *gap_ct = 0;
     return -1;
}
