#include <math.h>
#include <string.h>

#include "rb_gap_tree.h"
#include "gap_enum.h"

rb_gap_tree_t *gap_xfrm(rb_gap_t W, rb_gap_tree_t *gaps, task_t *tasks, int j)
{
     // Get the number of jobs for the task that will occur within W.
     size_t jobs = ceil(((double)W.exit - (double)tasks[j].r)/(double)tasks[j].p);
     size_t q;
     for (q = 1; q <= jobs; q++)
     {
	  // Initialize the time counter to the current job's release time.
	  rb_time_t t = tasks[j].r + (q-1) * tasks[j].p;
	  rb_gap_node_t *k_gap = tree_minimum(gaps, gaps->root);
	  while (k_gap != gaps->nil)
	  {
	       rb_time_t t1 = k_gap->gap.entry;
	       rb_time_t t2 = k_gap->gap.exit;
	       // No gap exists that can accommodate the current job. Bail.
	       if (t1 > t + tasks[j].p) return NULL;

	       // Bump effective release time to current gap under examination.
	       if (t < t1) t = t1;
	       // Check if effective release time falls w/in the current gap.
	       if ((t1 <= t) && (t < t2))
	       {
		    // Remove the gap from the tree.
		    rb_gap_node_t *free_gap = rb_delete(gaps, k_gap);
		    // Clean up the memory occupied by the spliced node.
		    if (free_gap) free(free_gap);
		    free_gap = NULL;
		    // Job fits with slack at the start.
		    if (t + tasks[j].c == t2)
		    {
			 rb_insert(gaps, (rb_gap_t){ t1, t });
		    }
		    // Job fits with slack on either side.
		    if (t + tasks[j].c < t2)
		    {
			 rb_insert(gaps, (rb_gap_t){t1, t});
			 rb_insert(gaps, (rb_gap_t){t + tasks[j].c, t2});
		    }
		    // Job does not fit. Model the abort.
		    if (t + tasks[j].c > t2)
		    {
			 rb_insert(gaps, (rb_gap_t){t1, t});
		    }
	       }
	       k_gap = tree_successor(gaps, k_gap);
	  }
     }

     // Return the modified gap tree.
     return gaps;
}

void _gap_srch(rb_gap_tree_t *T, rb_gap_node_t *X, rb_time_t c, rb_gap_t *gap)
{
     if (X != T->nil)
     {
	  if (gap->entry < 0) _gap_srch(T, X->left, c, gap);
	  if ((gap->entry < 0) && (c <= X->gap.exit - X->gap.entry))
	  {
	       gap->entry = X->gap.entry;
	       gap->exit = X->gap.exit;
	  }
	  if (gap->entry < 0) _gap_srch(T, X->right, c, gap);
     }
}

rb_gap_t gap_srch(rb_gap_tree_t *gaps, rb_time_t c)
{
     rb_gap_t gap = { -1, 0 };
     _gap_srch(gaps, gaps->root, c, &gap);
     return gap;
}

rb_time_t gap_enum(task_t *tasks, size_t n, size_t j, int windows)
{
     rb_time_t w = (rb_time_t)ceil((double)tasks[j].p/(double)windows);
     rb_time_t L = w;
     rb_time_t U = tasks[j].p + w;
     char buf[8192];
     while (L < U)
     {
	  rb_gap_tree_t *gaps = alloc_rb_gap_tree();
	  rb_insert(gaps, (rb_gap_t){ 0, tasks[j].p });
	  int i;
	  for (i = n-1; i > j; i--)
	  {
	       rb_gap_tree_t *gaps2 = gap_xfrm((rb_gap_t){ 0, L }, gaps, tasks, i);
	       if (!gaps2)
	       {
		    memset(buf, 0, 8192);
		    sprintf_rb_gap_tree(buf, gaps);
		    printf("%d: %s\n", i, buf);
		    free_rb_gap_tree(gaps);
		    return -1;
	       }
	       memset(buf, 0, 8192);
	       sprintf_rb_gap_tree(buf, gaps);
	       printf("%d: %s\n", i, buf);
	  }
	  memset(buf, 0, 8192);
	  sprintf_rb_gap_tree(buf, gaps);
	  printf("%d: %s\n", i, buf);
	  rb_gap_t gap = gap_srch(gaps, tasks[j].c);
	  rb_time_t rt_j;
	  if (gap.entry >= 0) rt_j = gap.entry + tasks[j].c;
	  if (rt_j < tasks[j].p)
	  {
	       free_rb_gap_tree(gaps);
	       return rt_j;
	  }
	  free_rb_gap_tree(gaps);
	  L += w;
     }
     return -1;
}
