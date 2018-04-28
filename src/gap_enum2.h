#ifndef __GAP_ENUM2_H
#define __GAP_ENUM2_H

#include "gap_list.h"

gap_list_t *gap_xfrm2(gap_t W, gap_list_t *gaps, task_t *tasks, int j, size_t *comp_steps);
gap_t gap_srch2(gap_list_t *gaps, task_time_t c, size_t *comp_steps);
task_time_t gap_enum2(task_t *tasks, size_t n, size_t j, int windows, size_t *comp_steps, size_t *gap_ct);

#endif
