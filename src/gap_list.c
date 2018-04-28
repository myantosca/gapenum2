#include "gap_list.h"

gap_list_t *alloc_gap_list()
{
     gap_list_t *LL = (gap_list_t *)malloc(sizeof(gap_list_t));
     LL->tail = malloc(sizeof(gap_list_node_t));
     LL->tail->gap.entry = -1;
     LL->tail->gap.exit = 0;
     LL->tail->prev = LL->tail;
     LL->tail->next = LL->tail;
     LL->head = LL->tail;
}

size_t gl_size(gap_list_t *LL)
{
     size_t len = 0;
     if (LL)
     {
	  gap_list_node_t* cursor = LL->head;

	  while (cursor != LL->tail)
	  {
	       cursor = cursor->next;
	       len++;
	  }
     }
     return len;
}


gap_list_node_t *gl_delete(gap_list_t *LL, gap_list_node_t *Y, size_t *comp_steps)
{
     if (LL && Y && Y != LL->tail)
     {
	  (*comp_steps)++;
	  gap_list_node_t *X = Y->prev;
	  gap_list_node_t *Z = Y->next;
	  X->next = Z;
	  Z->prev = X;
	  if (X == LL->tail) LL->head = Z;
     }
     return Y;
}

void gl_insert(gap_list_t *LL, gap_list_node_t *Z, gap_t gap, size_t *comp_steps)
{
     if (LL && Z)
     {
	  (*comp_steps)++;
	  gap_list_node_t *Y = (gap_list_node_t *)malloc(sizeof(gap_list_node_t));
	  gap_list_node_t *X = Z->prev;
	  Y->gap.entry = gap.entry;
	  Y->gap.exit = gap.exit;
	  Y->prev = X;
	  Y->next = Z;
	  if (X == LL->tail) LL->head = Y;
	  Z->prev = Y;
	  X->next = Y;
     }
}


void free_gap_list(gap_list_t *LL)
{
     if (LL)
     {
	  gap_list_node_t* cursor = LL->head;
	  while (cursor != LL->tail)
	  {
	       gap_list_node_t* free_me = cursor;
	       cursor = cursor->next;
	       free(free_me);
	       free_me = NULL;
	  }
	  free(LL->tail);
	  free(LL);
     }
}

size_t sprintf_gap_list(char *buf, gap_list_t *LL)
{
     if (LL)
     {
	  buf += sprintf(buf, "(");
	  gap_list_node_t* cursor = LL->head;
	  while (cursor != LL->tail)
	  {
	       buf += sprintf(buf, "{%ld,%ld}", cursor->gap.entry, cursor->gap.exit);
	       cursor = cursor->next;
	  }
	  buf += sprintf(buf, ")");
     }
}
