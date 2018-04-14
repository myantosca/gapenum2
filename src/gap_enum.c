#include <math.h>

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
	  // Get an in-order, by-item iterator for the tree.
	  rb_gap_tree_iterator_t iter = rb_get_iter(gaps);
	  // Loop over the tree nodes (k-gaps).
	  int k;
	  for (k = 0; k < iter.size; k++)
	  {
	       rb_gap_tree_t *k_gap = iter.nodes[k];
	       rb_time_t t1 = k_gap->gap.entry;
	       rb_time_t t2 = k_gap->gap.exit;
	       // No gap exists that can accommodate the current job. Bail.
	       if (t1 < t + tasks[j].p) return NULL;

	       // Bump effective release time to current gap under examination.
	       if (t < t1) t = t1;
	       // Check if effective release time falls w/in the current gap.
	       if ((t1 <= t) && (t < t2))
	       {
		    // Remove the gap from the tree.
		    gaps = rb_delete(gaps, k_gap);
		    // Clean up the memory occupied by the node.
		    free(k_gap);
		    // Job fits with slack at the start.
		    if (t + tasks[j].c == t2)
		    {
			 gaps = rb_insert(gaps, (rb_gap_t){ t1, t });
			 break;
		    }
		    // Job fits with slack on either side.
		    if (t + tasks[j].c < t2)
		    {
			 gaps = rb_insert(gaps, (rb_gap_t){t1, t});
			 gaps = rb_insert(gaps, (rb_gap_t){t, t2});
			 break;
		    }
		    // Job fits with slack at the end.
		    if (t + tasks[j].c > t2)
		    {
			 gaps = rb_insert(gaps, (rb_gap_t){t1, t2});
		    }
	       }
	  }
	  free(iter.nodes);
     }

     // Return the modified gap tree.
     return gaps;
}

rb_gap_tree_t *_gap_srch(rb_gap_tree_t *gaps, rb_time_t c, rb_gap_t *gap)
{
     if (gaps)
     {
	  if (gap->entry < 0) _gap_srch(gaps->left, c, gap);
	  if ((gap->entry < 0) && (c <= gaps->gap.exit - gaps->gap.entry))
	  {
	       gap->entry = gaps->gap.entry;
	       gap->exit = gaps->gap.exit;
	  }
	  if (gap->entry < 0) _gap_srch(gaps->right, c, gap);
     }
}

rb_gap_t gap_srch(rb_gap_tree_t *gaps, rb_time_t c)
{
     rb_gap_t gap = { -1, 0 };
     _gap_srch(gaps, c, &gap);
     return gap;
}

rb_time_t gap_enum(task_t *tasks, int j, int windows)
{
}
