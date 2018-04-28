#ifndef __RB_GAP_TREE_H
#define __RB_GAP_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include "task.h"

typedef enum rb_color
{
  BLACK = 0,
  RED = 1
} rb_color_t;

typedef int64_t task_time_t;

typedef struct rb_gap
{
  task_time_t entry;
  task_time_t exit;
} rb_gap_t;

typedef struct rb_gap_node
{
  rb_color_t color;
  rb_gap_t gap;
  struct rb_gap_node *parent;
  struct rb_gap_node *left;
  struct rb_gap_node *right;
} rb_gap_node_t;

typedef struct rb_gap_tree
{
  rb_gap_node_t *root;
  rb_gap_node_t *nil;
} rb_gap_tree_t;

/* typedef struct rb_gap_tree_iterator */
/* { */
/*   size_t size; */
/*   rb_gap_tree_t **nodes; */

/* } rb_gap_tree_iterator_t; */

rb_gap_tree_t *alloc_rb_gap_tree();

rb_gap_node_t *tree_minimum(rb_gap_tree_t *T, rb_gap_node_t *X, size_t *comp_steps);
rb_gap_node_t *tree_successor(rb_gap_tree_t *T, rb_gap_node_t *X, size_t *comp_steps);
void rb_left_rotate(rb_gap_tree_t *T, rb_gap_node_t *X);
void rb_right_rotate(rb_gap_tree_t *T, rb_gap_node_t *Y);
void rb_insert(rb_gap_tree_t *T, rb_gap_t g, size_t *comp_steps);
rb_gap_node_t *rb_delete(rb_gap_tree_t *T, rb_gap_node_t *Z, size_t *comp_steps);
size_t rb_size(rb_gap_tree_t *T);
void free_rb_gap_tree(rb_gap_tree_t *T);
size_t sprintf_rb_gap_tree(char *buf, rb_gap_tree_t *T);

#endif
