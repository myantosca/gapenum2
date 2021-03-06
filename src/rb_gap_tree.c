#include "rb_gap_tree.h"
#include <string.h>
#include <assert.h>


rb_gap_tree_t *alloc_rb_gap_tree()
{
     rb_gap_tree_t *T = malloc(sizeof(rb_gap_tree_t));
     T->nil = malloc(sizeof(rb_gap_node_t));
     T->nil->color = BLACK;
     T->nil->gap = (gap_t){ -1, 0 };
     T->nil->parent = T->nil;
     T->nil->left = T->nil;
     T->nil->right = T->nil;
     T->root = T->nil;
}


rb_gap_node_t *tree_minimum(rb_gap_tree_t *T, rb_gap_node_t *X, size_t *comp_steps)
{
     if (!T || !X) return NULL;
     while (X->left != T->nil)
     {
	  (*comp_steps)++;
	  X = X->left;
     }
     return X;
}

rb_gap_node_t *tree_successor(rb_gap_tree_t *T, rb_gap_node_t *X, size_t *comp_steps)
{
     if (!T || !X) return NULL;
     if (X->right != T->nil) return tree_minimum(T, X->right, comp_steps);
     rb_gap_node_t *Y = X->parent;
     while ((Y != T->nil) && (X == Y->right))
     {
	  (*comp_steps)++;
	  X = Y;
	  Y = Y->parent;
     }
     return Y;
}

void rb_left_rotate(rb_gap_tree_t *T, rb_gap_node_t *X)
{
     if (!T || !X) return;
     rb_gap_node_t *Y = X->right;
     X->right = Y->left;
     if (Y->left != T->nil) Y->left->parent = X;
     Y->parent = X->parent;
     if (X->parent == T->nil)
     {
	  T->root = Y;
     }
     else if (X == X->parent->left)
     {
	  X->parent->left = Y;
     }
     else
     {
	  X->parent->right = Y;
     }
     Y->left = X;
     X->parent = Y;
}

void rb_right_rotate(rb_gap_tree_t *T, rb_gap_node_t *Y)
{
     if (!T || !Y) return;
     rb_gap_node_t *X = Y->left;
     Y->left = X->right;
     if (X->right != T->nil) X->right->parent = Y;
     X->parent = Y->parent;
     if (Y->parent == T->nil)
     {
	  T->root = X;
     }
     else if (Y == Y->parent->left)
     {
	  Y->parent->left = X;
     }
     else
     {
	  Y->parent->right = X;
     }
     X->right = Y;
     Y->parent = X;
}

void rb_insert(rb_gap_tree_t *T, gap_t g, size_t *comp_steps)
{
     // Binary tree insertion
     rb_gap_node_t *Y = T->nil;
     rb_gap_node_t *X = T->root;
     rb_gap_node_t *Z = malloc(sizeof(rb_gap_node_t));
     Z->gap = g;
     Z->left = T->nil;
     Z->right = T->nil;

     while (X != T->nil)
     {
	  (*comp_steps)++;
	  Y = X;
	  X = (Z->gap.entry < X->gap.entry) ? X->left : X->right;
     }
     Z->parent = Y;

     if (Y == T->nil)
     {
	  T->root = Z;
     }
     else if (Z->gap.entry < Y->gap.entry)
     {
	  Y->left = Z;
     }
     else
     {
	  Y->right = Z;
     }

     // Red-black balancing
     Z->color = RED;
     while ((Z != T->root) && (Z->parent->color == RED))
     {
	  (*comp_steps)++;
	  if (Z->parent == Z->parent->parent->left)
	  {
	       Y = Z->parent->parent->right;
	       if (Y->color == RED)
	       {
		    Z->parent->color = BLACK;
		    Y->color = BLACK;
		    Z->parent->parent->color = RED;
		    Z = Z->parent->parent;
	       }
	       else
	       {
		    if (Z == Z->parent->right)
		    {
			 Z = Z->parent;
			 rb_left_rotate(T, Z);
		    }
		    Z->parent->color = BLACK;
		    Z->parent->parent->color = RED;
		    rb_right_rotate(T, Z->parent->parent);
	       }
	  }
	  else
	  {
	       Y = Z->parent->parent->left;
	       if (Y->color == RED)
	       {
		    Z->parent->color = BLACK;
		    Y->color = BLACK;
		    Z->parent->parent->color = RED;
		    Z = Z->parent->parent;
	       }
	       else
	       {
		    if (Z == Z->parent->left)
		    {
			 Z = Z->parent;
			 rb_right_rotate(T, Z);
		    }
		    Z->parent->color = BLACK;
		    Z->parent->parent->color = RED;
		    rb_left_rotate(T, Z->parent->parent);
	       }
	  }
     }
     T->root->color = BLACK;
}

void rb_delete_fixup(rb_gap_tree_t *T, rb_gap_node_t *X, size_t *comp_steps)
{
     while ((X != T->root) && (X->color == BLACK))
     {
	  (*comp_steps)++;
	  rb_gap_node_t *W = T->nil;
	  if (X == X->parent->left)
	  {
	       W = X->parent->right;
	       if (W->color == RED)
	       {
		    W->color = BLACK;
		    X->parent->color = RED;
		    rb_left_rotate(T, X->parent);
		    W = X->parent->right;
	       }
	       if ((W->left->color == BLACK) && (W->right->color == BLACK))
	       {
		    W->color = RED;
		    X = X->parent;
	       }
	       else
	       {
		    if (W->right->color == BLACK)
		    {
			 W->left->color = BLACK;
			 W->color = RED;
			 rb_right_rotate(T, W);
			 W = X->parent->right;
		    }
		    W->color = X->parent->color;
		    X->parent->color = BLACK;
		    W->right->color = BLACK;
		    rb_left_rotate(T, X->parent);
		    X = T->root;
	       }
	  }
	  else
	  {
	       W = X->parent->left;
	       if (W->color == RED)
	       {
		    W->color = BLACK;
		    X->parent->color = RED;
		    rb_right_rotate(T, X->parent);
		    W = X->parent->left;
	       }
	       if ((W->right->color == BLACK) && (W->left->color == BLACK))
	       {
		    W->color = RED;
		    X = X->parent;
	       }
	       else
	       {
		    if (W->left->color == BLACK)
		    {
			 W->right->color = BLACK;
			 W->color = RED;
			 rb_left_rotate(T, W);
			 W = X->parent->left;
		    }
		    W->color = X->parent->color;
		    X->parent->color = BLACK;
		    W->left->color = BLACK;
		    rb_right_rotate(T, X->parent);
		    X = T->root;
	       }
	  }
     }
     if (X) X->color = BLACK;
}

rb_gap_node_t *rb_delete(rb_gap_tree_t *T, rb_gap_node_t *Z, size_t *comp_steps)
{
     if (!T || !Z) return NULL;
     rb_gap_node_t *Y = T->nil;
     rb_gap_node_t *X = T->nil;
     Y = ((Z->left == T-> nil) || (Z->right == T->nil)) ? Z : tree_successor(T, Z, comp_steps);
     X = (Y->left != T->nil) ? Y->left : Y->right;
     X->parent = Y->parent;
     if (Y->parent == T->nil)
     {
	  T->root = X;
     }
     else if (Y == Y->parent->left)
     {
	  Y->parent->left = X;
     }
     else
     {
	  Y->parent->right = X;
     }
     if (Y != Z)
     {
	  Z->gap = Y->gap;
     }
     if (Y->color == BLACK) rb_delete_fixup(T, X, comp_steps);
     return Y;
}

void _free_rb_gap_tree(rb_gap_tree_t *T, rb_gap_node_t *X)
{
     if (X != T->nil)
     {
	  rb_gap_node_t *L = X->left;
	  rb_gap_node_t *R = X->right;
	  _free_rb_gap_tree(T, L);
	  if (X) free(X);
	  _free_rb_gap_tree(T, R);
     }
}

void free_rb_gap_tree(rb_gap_tree_t *T)
{
     if (T)
     {
	  _free_rb_gap_tree(T, T->root);
	  if (T->nil) free(T->nil);
	  free(T);
     }
}

size_t _rb_size(rb_gap_tree_t *T, rb_gap_node_t *X)
{
     size_t n = 0;
     if (X != T->nil)
     {
	  n += _rb_size(T, X->left);
	  n++;
	  n += _rb_size(T, X->right);
     }
     return n;
}

size_t rb_size(rb_gap_tree_t *T)
{
     return _rb_size(T, T->root);
}

size_t _sprintf_rb_gap_tree(char *buf, rb_gap_tree_t *T, rb_gap_node_t *X)
{
     char *beg = buf;
     if (X != T->nil)
     {
	  buf += sprintf(buf, "(");
	  buf += _sprintf_rb_gap_tree(buf, T, X->left);
	  buf += sprintf(buf, "%c{%ld,%ld}", X->color ? 'R' : 'B', X->gap.entry, X->gap.exit);
	  buf += _sprintf_rb_gap_tree(buf, T, X->right);
	  buf += sprintf(buf, ")");
     }
     return buf - beg;
}

size_t sprintf_rb_gap_tree(char *buf, rb_gap_tree_t *T)
{
     return _sprintf_rb_gap_tree(buf, T, T->root);
}
