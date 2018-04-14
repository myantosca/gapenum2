#include "gap_enum.h"

rb_gap_tree_t *gap_xfrm(rb_gap_tree_t *gaps, task_t *tasks)
{
     return NULL;
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
     return -1;
}
