#include <math.h>
#include <string.h>

#include "rb_gap_tree.h"
#include "gap_enum2.h"

rb_gap_tree_t *gap_xfrm2(gap_t W, rb_gap_tree_t *gaps, task_t *tasks, int j, size_t *comp_steps)
{
     // Get the number of jobs for the task that will occur within W.
     size_t jobs = ceil(((double)W.exit - (double)tasks[j].r)/(double)tasks[j].p);
     size_t q;
     char buf[8192];
     for (q = 1; q <= jobs; q++)
     {
	  // Initialize the time counter to the current job's release time.
	  task_time_t t = tasks[j].r + (q-1) * tasks[j].p;
	  rb_gap_node_t *k_gap = tree_minimum(gaps, gaps->root, comp_steps);
	  while (k_gap != gaps->nil)
	  {
	       (*comp_steps)++;
	       task_time_t t1 = k_gap->gap.entry;
	       task_time_t t2 = k_gap->gap.exit;
	       rb_gap_node_t *free_gap = NULL;
	       // No gap exists that can accommodate the current job. Bail.
	       if (t1 > t + tasks[j].p) return NULL;

	       // Bump effective release time to current gap under examination.
	       if (t < t1) t = t1;
	       // Check if effective release time falls w/in the current gap.
	       if ((t1 <= t) && (t < t2))
	       {
		    // Remove the gap from the tree.
		    free_gap = rb_delete(gaps, k_gap, comp_steps);
		    // Job fits with slack at the start.
		    if (t + tasks[j].c == t2)
		    {
			 if (t > t1) rb_insert(gaps, (gap_t){ t1, t }, comp_steps);
			 // Clean up the memory occupied by the spliced node.
			 if (free_gap) free(free_gap);
			 free_gap = NULL;
			 break;
		    }
		    // Job fits with slack on either side.
		    if (t + tasks[j].c < t2)
		    {
			 if (t > t1) rb_insert(gaps, (gap_t){t1, t}, comp_steps);
			 rb_insert(gaps, (gap_t){t + tasks[j].c, t2}, comp_steps);
			 // Clean up the memory occupied by the spliced node.
			 if (free_gap) free(free_gap);
			 free_gap = NULL;
			 break;
		    }
		    // Job does not fit. Model the abort.
		    if (t + tasks[j].c > t2)
		    {
			 if (t > t1) rb_insert(gaps, (gap_t){t1, t}, comp_steps);
		    }
	       }

	       if (k_gap->gap.entry == t1) k_gap = tree_successor(gaps, k_gap, comp_steps);
	       // Clean up the memory occupied by the spliced node.
	       if (free_gap) free(free_gap);
	       free_gap = NULL;
	  }
     }

     // Return the modified gap tree.
     return gaps;
}

void _gap_srch2(rb_gap_tree_t *T, rb_gap_node_t *X, task_time_t c, gap_t *gap, size_t *comp_steps)
{
     if (X != T->nil)
     {
	  if (gap->entry < 0) _gap_srch2(T, X->left, c, gap, comp_steps);
	  if (gap->entry < 0)
	  {
	       if (c <= X->gap.exit - X->gap.entry)
	       {
		    (*comp_steps)++;
		    gap->entry = X->gap.entry;
		    gap->exit = X->gap.exit;
	       }
	  }
	  else
	  {
	       (*comp_steps)++;
	  }
	  if (gap->entry < 0) _gap_srch2(T, X->right, c, gap, comp_steps);
     }
}

gap_t gap_srch2(rb_gap_tree_t *gaps, task_time_t c, size_t *comp_steps)
{
     gap_t gap = { -1, 0 };
     _gap_srch2(gaps, gaps->root, c, &gap, comp_steps);
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
	  rb_gap_tree_t *gaps = alloc_rb_gap_tree();
	  rb_insert(gaps, (gap_t){ 0, L }, comp_steps);
	  int i;
	  for (i = n-1; i > j; i--)
	  {
	       (*comp_steps)++;
	       rb_gap_tree_t *gaps2 = gap_xfrm2((gap_t){ 0, L }, gaps, tasks, i, comp_steps);
	       if (!gaps2)
	       {
		    *gap_ct = rb_size(gaps);
		    memset(buf, 0, 8192);
		    sprintf_rb_gap_tree(buf, gaps);
		    fprintf(stderr, "%d: %s\n", i, buf);
		    free_rb_gap_tree(gaps);
		    return -1;
	       }
	  }
	  gap_t gap = gap_srch2(gaps, tasks[j].c, comp_steps);
	  task_time_t rt_j = -1;
	  if (gap.entry >= 0) rt_j = gap.entry + tasks[j].c;
	  if (rt_j < tasks[j].p)
	  {
	       *gap_ct = rb_size(gaps);
	       memset(buf, 0, 8192);
	       sprintf_rb_gap_tree(buf, gaps);
	       fprintf(stderr, "%d: %s\n", i, buf);
	       free_rb_gap_tree(gaps);
	       return rt_j;
	  }
	  free_rb_gap_tree(gaps);
	  L += w;
     }
     *gap_ct = 0;
     return -1;
}
