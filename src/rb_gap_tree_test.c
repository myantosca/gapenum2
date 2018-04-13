#include <stdlib.h>
#include <stdio.h>

#include "rb_gap_tree.h"


void print_rb_gap_tree_as_list(rb_gap_tree_t *T)
{
     if (T)
     {
	  print_rb_gap_tree_as_list(T->left);
	  printf("(%ld,%ld) ", T->gap.entry, T->gap.exit);
	  print_rb_gap_tree_as_list(T->right);
     }
}

int main()
{
  rb_gap_tree_t *T = NULL;
  print_rb_gap_tree_as_list(T);
  printf("\n");
  T = rb_insert(T, (rb_gap_t){ 0, 1 });
  print_rb_gap_tree_as_list(T);
  printf("\n");
  T = rb_insert(T, (rb_gap_t){ 1, 2 });
  print_rb_gap_tree_as_list(T);
  printf("\n");
  T = rb_insert(T, (rb_gap_t){ 2, 3 });
  print_rb_gap_tree_as_list(T);
  printf("\n");
  rb_delete(T, T);
  print_rb_gap_tree_as_list(T);  
  printf("\n");
  return 0;
}
