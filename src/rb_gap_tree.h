#ifndef __RB_GAP_TREE_H
#define __RB_GAP_TREE_H

#include <stdlib.h>
#include <sys/types.h>

typedef enum rb_color
{
  BLACK = 0,
  RED = 1
} rb_color_t;

typedef int64_t rb_time_t;

typedef struct rb_gap
{
  rb_time_t entry;
  rb_time_t exit;
} rb_gap_t;

typedef struct rb_gap_tree
{
  rb_color_t color;
  rb_gap_t gap;
  struct rb_gap_tree *parent;
  struct rb_gap_tree *left;
  struct rb_gap_tree *right;  
} rb_gap_tree_t;

rb_gap_tree_t *tree_minimum(rb_gap_tree_t *X);
rb_gap_tree_t *tree_successor(rb_gap_tree_t *X);
rb_gap_tree_t *rb_left_rotate(rb_gap_tree_t *T, rb_gap_tree_t *X);
rb_gap_tree_t *rb_right_rotate(rb_gap_tree_t *T, rb_gap_tree_t *Y); 
rb_gap_tree_t *rb_insert(rb_gap_tree_t *T, rb_gap_t g);
rb_gap_tree_t *rb_delete(rb_gap_tree_t *T, rb_gap_tree_t *Z);
void rb_free(rb_gap_tree_t *T);

#endif
