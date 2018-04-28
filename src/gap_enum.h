#ifndef __GAP_ENUM_H
#define __GAP_ENUM_H

#include "rb_gap_tree.h"

rb_gap_tree_t *gap_xfrm(rb_gap_t W, rb_gap_tree_t *gaps, task_t *tasks, int j, size_t *comp_steps);
rb_gap_t gap_srch(rb_gap_tree_t *gaps, task_time_t c, size_t *comp_steps);
task_time_t gap_enum(task_t *tasks, size_t n, size_t j, int windows, size_t *comp_steps, size_t *gap_ct);

#endif
