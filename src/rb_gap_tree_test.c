#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rb_gap_tree.h"


size_t sprintf_rb_gap_tree(char *buf, rb_gap_tree_t *T)
{
     char *beg = buf;
     if (T)
     {
	  buf += sprintf(buf, "(");
	  buf += sprintf_rb_gap_tree(buf, T->left);
	  buf += sprintf(buf, "%c{%ld,%ld}", T->color ? 'R' : 'B', T->gap.entry, T->gap.exit);
	  buf += sprintf_rb_gap_tree(buf, T->right);
	  buf += sprintf(buf, ")");
     }
     return buf - beg;
}

int assert_rb_gap_tree(const char *label, const char* truth, rb_gap_tree_t *T)
{
     char buf[1024];     
     memset(buf,0,1024);
     sprintf_rb_gap_tree(buf, T);
     int ok = !strcmp(truth, buf);
     printf("%s, %s: %s == %s\n", ok ? "PASS" : "FAIL", label, truth, buf);
     return ok;
}

int main()
{
     rb_gap_tree_t *T = NULL;
     assert_rb_gap_tree("empty tree", "", T);

     T = rb_insert(T, (rb_gap_t){ 0, 1 });
     assert_rb_gap_tree("insert empty", "(B{0,1})", T);

     T = rb_insert(T, (rb_gap_t){ 1, 2 });
     assert_rb_gap_tree("insert right", "(B{0,1}(R{1,2}))", T);

     T = rb_insert(T, (rb_gap_t){ 2, 3 });
     assert_rb_gap_tree("insert right", "((R{0,1})B{1,2}(R{2,3}))", T);

     T = rb_delete(T, T);
     assert_rb_gap_tree("delete root (3)", "(B{0,1}(R{2,3}))", T);

     T = rb_delete(T, T);
     assert_rb_gap_tree("delete root (2)", "(B{2,3})", T);

     T = rb_delete(T, T);
     assert_rb_gap_tree("delete root (1)", "", T);

  return 0;
}
