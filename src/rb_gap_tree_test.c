#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rb_gap_tree.h"

int assert_rb_gap_tree_form(const char *label, const char* truth, rb_gap_tree_t *T)
{
     char buf[1024];
     memset(buf,0,1024);
     sprintf_rb_gap_tree(buf, T);
     int ok = !strcmp(truth, buf);
     printf("%s, %s: %s == %s\n", ok ? "PASS" : "FAIL", label, truth, buf);
     return ok;
}

int _assert_rb_gap_tree_props(size_t size, const rb_gap_t *members, rb_gap_tree_t *T, int bh, int *i)
{
     int ok;
     if (!T)
     {
	  ok = (bh == 0);
     }
     else
     {
	  ok = _assert_rb_gap_tree_props(size, members, T->left, bh - (T->color == BLACK ? 1 : 0), i);
	  ok = ok &&
	       ((T->color == BLACK) ||
		((T->color == RED) &&
		 ((!T->left) || (T->left->color == BLACK)) &&
		 ((!T->right) || (T->right->color == BLACK)))) &&
	       (T->gap.entry == members[*i].entry) &&
	       (T->gap.exit == members[*i].exit);
	  (*i)++;
	  ok = ok && _assert_rb_gap_tree_props(size, members, T->right, bh - (T->color == BLACK ? 1 : 0), i);
     }
     return ok;
}

int assert_rb_gap_tree_props(const char *label, size_t size, const rb_gap_t *members, rb_gap_tree_t *T)
{
     rb_gap_tree_t *U = T;
     int bh = 0;
     while (U)
     {
	  bh += (U->color == BLACK) ? 1 : 0;
	  U = U->left;
     }
     int i = 0;
     int ok = _assert_rb_gap_tree_props(size, members, T, bh, &i);
     ok = ok && (size == i);
     printf("[%s] %s\n", ok ? "PASS" : "FAIL", label);
     return ok;
}

const rb_gap_t members_0_to_8[8] = {{0,1},{1,2},{2,3},{4,5},{5,6},{6,7},{7,8}};

int main()
{
     rb_gap_tree_t *T = NULL;
     assert_rb_gap_tree_props("empty tree", 0, NULL, T);

     T = rb_insert(T, (rb_gap_t){ 0, 1 });
     assert_rb_gap_tree_props("insert empty", 1, members_0_to_8, T);

     T = rb_insert(T, (rb_gap_t){ 1, 2 });
     assert_rb_gap_tree_props("insert right (1)", 2, members_0_to_8, T);

     T = rb_insert(T, (rb_gap_t){ 2, 3 });
     assert_rb_gap_tree_props("insert right (2)", 3, members_0_to_8, T);

     T = rb_insert(T, (rb_gap_t){ 4, 5 });
     assert_rb_gap_tree_props("insert right (3)", 4, members_0_to_8, T);

     T = rb_insert(T, (rb_gap_t){ 5, 6 });
     assert_rb_gap_tree_props("insert right (4)", 5, members_0_to_8, T);

     T = rb_insert(T, (rb_gap_t){ 6, 7 });
     assert_rb_gap_tree_props("insert right (5)", 6, members_0_to_8, T);

     T = rb_insert(T, (rb_gap_t){ 7, 8 });
     assert_rb_gap_tree_props("insert right (6)", 7, members_0_to_8, T);

     rb_free(T);

     T = NULL;

     return 0;
}
