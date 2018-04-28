#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rb_gap_tree.h"

int _assert_rb_gap_tree_props(size_t size, const gap_t *members, rb_gap_tree_t *T, rb_gap_node_t *X, int bh, int *i)
{
     int ok;
     if (X == T->nil)
     {
	  ok = (bh == 0);
     }
     else
     {
	  ok = _assert_rb_gap_tree_props(size, members, T, X->left, bh - (X->color == BLACK ? 1 : 0), i);
	  ok = ok &&
	       ((X->color == BLACK) ||
		((X->color == RED) &&
		 ((!X->left) || (X->left->color == BLACK)) &&
		 ((!X->right) || (X->right->color == BLACK)))) &&
	       (X->gap.entry == members[*i].entry) &&
	       (X->gap.exit == members[*i].exit);
	  (*i)++;
	  ok = ok && _assert_rb_gap_tree_props(size, members, T, X->right, bh - (X->color == BLACK ? 1 : 0), i);
     }
     return ok;
}

int assert_rb_gap_tree_props(const char *label, size_t size, const gap_t *members, rb_gap_tree_t *T)
{
     rb_gap_node_t *U = T->root;
     int bh = 0;
     while (U != T->nil)
     {
	  bh += (U->color == BLACK) ? 1 : 0;
	  U = U->left;
     }
     int i = 0;
     int ok = _assert_rb_gap_tree_props(size, members, T, T->root, bh, &i);
     ok = ok && (size == i);
     printf("[%s] %s\n", ok ? "PASS" : "FAIL", label);
     return ok;
}

const gap_t members_0_to_7[8] = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,8}};

void print_rb_gap_tree(rb_gap_tree_t *T)
{
     char buf[8192];
     memset(buf, 0, 8192);
     sprintf_rb_gap_tree(buf, T);
     printf("%s\n", buf);
}

int main()
{
     rb_gap_tree_t *T = alloc_rb_gap_tree();
     print_rb_gap_tree(T);
     assert_rb_gap_tree_props("empty tree", 0, NULL, T);

     size_t comp_steps = 0;
     rb_insert(T, (gap_t){ 1, 2 }, &comp_steps);
     print_rb_gap_tree(T);
     assert_rb_gap_tree_props("insert right (1)", 1, members_0_to_7+1, T);

     rb_insert(T, (gap_t){ 2, 3 }, &comp_steps);
     print_rb_gap_tree(T);
     assert_rb_gap_tree_props("insert right (2)", 2, members_0_to_7+1, T);

     rb_insert(T, (gap_t){ 3, 4 }, &comp_steps);
     print_rb_gap_tree(T);
     assert_rb_gap_tree_props("insert right (3)", 3, members_0_to_7+1, T);

     rb_insert(T, (gap_t){ 4, 5 }, &comp_steps);
     print_rb_gap_tree(T);
     assert_rb_gap_tree_props("insert right (4)", 4, members_0_to_7+1, T);

     rb_insert(T, (gap_t){ 5, 6 }, &comp_steps);
     print_rb_gap_tree(T);
     assert_rb_gap_tree_props("insert right (5)", 5, members_0_to_7+1, T);

     rb_insert(T, (gap_t){ 6, 7 }, &comp_steps);
     print_rb_gap_tree(T);
     assert_rb_gap_tree_props("insert right (6)", 6, members_0_to_7+1, T);

     rb_insert(T, (gap_t){ 7, 8 }, &comp_steps);
     print_rb_gap_tree(T);
     assert_rb_gap_tree_props("insert right (7)", 7, members_0_to_7+1, T);

     rb_insert(T, (gap_t){ 0, 1 }, &comp_steps);
     print_rb_gap_tree(T);
     assert_rb_gap_tree_props("insert left (8)", 8, members_0_to_7, T);

     free_rb_gap_tree(T);

     T = NULL;

     return 0;
}
