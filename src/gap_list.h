#ifndef __GAP_LIST_H
#define __GAP_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include "task.h"

typedef struct gap_list_node
{
  gap_t gap;
  struct gap_list_node *prev;
  struct gap_list_node *next;
} gap_list_node_t;

typedef struct gap_list
{
  gap_list_node_t *head;
  gap_list_node_t *tail;
} gap_list_t;

gap_list_t *alloc_gap_list();
size_t gl_size(gap_list_t *LL);
gap_list_node_t *gl_delete(gap_list_t *LL, gap_list_node_t *Z, size_t *comp_steps);
void gl_insert(gap_list_t *LL, gap_list_node_t *Z, gap_t gap, size_t *comp_steps);
void free_gap_list(gap_list_t *LL);
size_t sprintf_gap_list(char *buf, gap_list_t *LL);

#endif
