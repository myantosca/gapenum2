#include "rb_gap_tree.h"


rb_gap_tree_t *tree_minimum(rb_gap_tree_t *X)
{
     if (!X) return NULL;
     while (X->left) X = X->left;
     return X;
}

rb_gap_tree_t *tree_successor(rb_gap_tree_t *X)
{
     if (!X) return NULL;
     rb_gap_tree_t *Y = NULL;
     if (X->right) return tree_minimum(X);
     Y = X->parent;
     while (Y && (X == Y->right))
     {
	  X = Y;
	  Y = Y->parent;
     }
     return Y;
}

rb_gap_tree_t *rb_left_rotate(rb_gap_tree_t *T, rb_gap_tree_t *X)
{
     if (!T || !X) return T;
     rb_gap_tree_t *Y = X->right;
     X->right = Y->left;
     if (Y->left) Y->left->parent = X;
     Y->parent = X->parent;
     if (!X->parent)
     {
	  T = Y;
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
     return T;
}

rb_gap_tree_t *rb_right_rotate(rb_gap_tree_t *T, rb_gap_tree_t *Y)
{
     if (!T || !Y) return T;
     rb_gap_tree_t *X = Y->left;
     Y->left = X->right;
     if (X->right) X->right->parent = Y;
     X->parent = Y->parent;
     if (!Y->parent)
     {
	  T = X;
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
     return T;
}

rb_gap_tree_t *rb_insert(rb_gap_tree_t *T, rb_gap_t g)
{
     // Binary tree insertion
     rb_gap_tree_t *Y = NULL;
     rb_gap_tree_t *X = T;
     rb_gap_tree_t *Z = malloc(sizeof(rb_gap_tree_t));
     Z->gap = g;
     Z->left = NULL;
     Z->right = NULL;
     while (X)
     {
	  Y = X;
	  X = (Z->gap.entry < X->gap.entry) ? X->left : X->right;
     }
     Z->parent = Y;
     if (!Y)
     {
	  T = Z;
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
     while ((Z != T) && (Z->parent->color == RED))
     {
	  if (Z->parent == Z->parent->parent->left)
	  {
	       Y = Z->parent->parent->right;
	       // Have to do the NULL check since leaves are black.
	       if (Y && Y->color == RED)
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
			 T = rb_left_rotate(T, Z);
		    }
		    Z->parent->color = BLACK;
		    Z->parent->parent->color = RED;
		    T = rb_right_rotate(T, Z->parent->parent);
	       }
	  }
	  else
	  {
	       Y = Z->parent->parent->left;
	       // Have to do the NULL check since leaves are black.
	       if (Y && Y->color == RED)
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
			 T = rb_right_rotate(T, Z);
		    }
		    Z->parent->color = BLACK;
		    Z->parent->parent->color = RED;
		    T = rb_left_rotate(T, Z->parent->parent);
	       }
	  }
     }
     T->color = BLACK;
     return T;
}

rb_gap_tree_t *rb_delete_fixup(rb_gap_tree_t *T, rb_gap_tree_t *X)
{
     while ((X != T) && (X->color == BLACK))
     {
	  rb_gap_tree_t *W = NULL;
	  if (X == X->parent->left)
	  {
	       W = X->parent->right;
	       if (W && W->color == RED)
	       {
		    W->color = BLACK;
		    X->parent->color = RED;
		    T = rb_left_rotate(T, X->parent);
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
			 T = rb_right_rotate(T, W);
			 W = X->parent->right;
		    }
		    W->color = X->parent->color;
		    X->parent->color = BLACK;
		    W->right->color = BLACK;
		    T = rb_left_rotate(T, X->parent);
		    X = T;
	       }
	  }
	  else
	  {
	       W = X->parent->left;
	       if (W->color == RED)
	       {
		    W->color = BLACK;
		    X->parent->color = RED;
		    T = rb_right_rotate(T, X->parent);
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
			 T = rb_left_rotate(T, W);
			 W = X->parent->left;
		    }
		    W->color = X->parent->color;
		    X->parent->color = BLACK;
		    W->left->color = BLACK;
		    T = rb_right_rotate(T, X->parent);
		    X = T;
	       }
	  }
     }
     if (X) X->color = BLACK;
     return T;
}

rb_gap_tree_t *rb_delete(rb_gap_tree_t *T, rb_gap_tree_t *Z)
{
     if (!T || !Z) return NULL;
     rb_gap_tree_t *Y = NULL;
     rb_gap_tree_t *X = NULL;
     Y = (!Z->left || !Z->right) ? Z : tree_successor(Z);
     X = Y->left ? Y->left : Y->right;
     if (X) X->parent = Y->parent;
     if (!Y->parent)
     {
	  T = X;
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
     if (Y->color == BLACK) T = rb_delete_fixup(T, X);
     return T;
}

void rb_free(rb_gap_tree_t *T)
{
     if (T)
     {
	  rb_gap_tree_t *L = T->left;
	  rb_gap_tree_t *R = T->right;
	  rb_free(L);
	  free(T);
	  rb_free(R);
     }
}

size_t rb_size(rb_gap_tree_t *T)
{
     size_t n = 0;
     if (T)
     {
	  n += rb_size(T->left);
	  n++;
	  n += rb_size(T->right);
     }
     return n;
}


void _rb_get_iter(rb_gap_tree_t *T, rb_gap_tree_iterator_t *iter, size_t *i)
{
     if (T)
     {
	  _rb_get_iter(T->left, iter, i);
	  iter->nodes[*i] = T;
	  *i++;
	  _rb_get_iter(T->right, iter, i);
     }
}

rb_gap_tree_iterator_t rb_get_iter(rb_gap_tree_t *T)
{
     rb_gap_tree_iterator_t iter;
     iter.size = rb_size(T);
     iter.nodes = malloc(sizeof(rb_gap_tree_t *) * iter.size);
     size_t i = 0;
     _rb_get_iter(T, &iter, &i);
     return iter;
}

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
