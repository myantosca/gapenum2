#ifndef __GAP_ENUM_H
#define __GAP_ENUM_H

#include "task.h"
#include "rb_gap_tree.h"

rb_gap_tree_t *gap_xfrm(rb_gap_t W, rb_gap_tree_t *gaps, task_t *tasks, int j);
rb_gap_t gap_srch(rb_gap_tree_t *gaps, rb_time_t c);
rb_time_t gap_enum(task_t *tasks, int n, int j, int windows);

#endif
