#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_ntudsa.h"

/* Creates and returns a new table
   with comparison function |compare| using parameter |param|
   and memory allocator |allocator|.
   Returns |NULL| if memory allocation failed. */
struct avl_table *
avl_create (avl_comparison_func *compare, void *param,
            struct libavl_allocator *allocator)
{
  struct avl_table *tree;

  assert (compare != NULL);

  if (allocator == NULL)
    allocator = &avl_allocator_default;

  tree = allocator->libavl_malloc (allocator, sizeof *tree);
  if (tree == NULL)
    return NULL;

  tree->avl_root = NULL;
  tree->avl_compare = compare;
  tree->avl_param = param;
  tree->avl_alloc = allocator;
  tree->avl_count = 0;
  tree->avl_generation = 0;

  return tree;
}

int *
avl_probe (struct avl_table *tree, int item) /* change code by Yen-Chieh */
{
  struct avl_node *y, *z; /* Top node to update balance factor, and parent. */
  struct avl_node *p, *q; /* Iterator, and parent. */
  struct avl_node *n;     /* Newly inserted node. */
  struct avl_node *w;     /* New root of rebalanced subtree. */
  int dir;                /* Direction to descend. */

  int tmp1, tmp2, tmp3; /* add code by Yen-Chieh */
  long long int t1, t2; /* add code by Yen-Chieh */

  unsigned char da[AVL_MAX_HEIGHT]; /* Cached comparison results. */
  int k = 0;              /* Number of cached results. */

  assert (tree != NULL);

  z = (struct avl_node *) &tree->avl_root;
  y = tree->avl_root;
  dir = 0;
  for (q = z, p = y; p != NULL; q = p, p = p->avl_link[dir])
    {
      int cmp = tree->avl_compare (&item, &p->avl_data, tree->avl_param);
      if (cmp == 0) {
        ++p->avl_cnt; /* add code by Yen-Chieh */
        return &p->avl_data;
      }

      if (p->avl_balance != 0)
        z = q, y = p, k = 0;
      da[k++] = dir = cmp > 0;
      p->avl_cnode[dir]++; /* add code by Yen-Chieh */
      p->avl_sum[dir] += (long long int)item; /* add code by Yen-Chieh */
    }

  n = q->avl_link[dir] =
    tree->avl_alloc->libavl_malloc (tree->avl_alloc, sizeof *n);

  if (n == NULL)
    return NULL;

  tree->avl_count++;
  n->avl_data = item;
  n->avl_cnt = 1;
  n->avl_link[0] = n->avl_link[1] = NULL;
  n->avl_cnode[0] = n->avl_cnode[1] = 0; /* add code by Yen-Chieh */
  n->avl_sum[0] = n->avl_sum[1] = 0ll; /* add code by Yen-Chieh */
  n->avl_balance = 0;
  if (y == NULL)
    return &n->avl_data;

  for (p = y, k = 0; p != n; p = p->avl_link[da[k]], k++)
    if (da[k] == 0)
      p->avl_balance--;
    else
      p->avl_balance++;


  tmp3 = y->avl_cnode[0] + y->avl_cnt + y->avl_cnode[1];
  if (y->avl_balance == -2)
    {

      struct avl_node *x = y->avl_link[0];
      if (x->avl_balance == -1)
        {
          w = x;
          y->avl_link[0] = x->avl_link[1];
          x->avl_link[1] = y;

          y->avl_cnode[0] = x->avl_cnode[1]; /* add code by Yen-Chieh */
          x->avl_cnode[1] = y->avl_cnode[0] + y->avl_cnt + y->avl_cnode[1]; /* add code by Yen-Chieh */

          y->avl_sum[0] = x->avl_sum[1]; /* add code by Yen-Chieh */
          x->avl_sum[1] = y->avl_sum[0] + (long long int)y->avl_cnt * y->avl_data + y->avl_sum[1]; /* add code by Yen-Chieh */

          x->avl_balance = y->avl_balance = 0;
        }
      else
        {

          assert (x->avl_balance == +1);
          w = x->avl_link[1];

          tmp1 = w->avl_cnode[0]; /* add code by Yen-Chieh */
          tmp2 = w->avl_cnode[1]; /* add code by Yen-Chieh */

          t1 = w->avl_sum[0]; /* add code by Yen-Chieh */
          t2 = w->avl_sum[1]; /* add code by Yen-Chieh */

          x->avl_link[1] = w->avl_link[0];
          w->avl_link[0] = x;
          y->avl_link[0] = w->avl_link[1];
          w->avl_link[1] = y;

          x->avl_cnode[1] = tmp1; /* add code by Yen-Chieh */
          y->avl_cnode[0] = tmp2; /* add code by Yen-Chieh */

          x->avl_sum[1] = t1; /* add code by Yen-Chieh */
          y->avl_sum[0] = t2; /* add code by Yen-Chieh */

          w->avl_cnode[0] = x->avl_cnode[0] + x->avl_cnt + x->avl_cnode[1]; /* add code by Yen-Chieh */
          w->avl_cnode[1] = y->avl_cnode[0] + y->avl_cnt + y->avl_cnode[1]; /* add code by Yen-Chieh */

          w->avl_sum[0] = x->avl_sum[0] + (long long int)x->avl_cnt * x->avl_data + x->avl_sum[1]; /* add code by Yen-Chieh */
          w->avl_sum[1] = y->avl_sum[0] + (long long int)y->avl_cnt * y->avl_data + y->avl_sum[1]; /* add code by Yen-Chieh */

          if (w->avl_balance == -1)
            x->avl_balance = 0, y->avl_balance = +1;
          else if (w->avl_balance == 0)
            x->avl_balance = y->avl_balance = 0;
          else /* |w->avl_balance == +1| */
            x->avl_balance = -1, y->avl_balance = 0;
          w->avl_balance = 0;
        }
    }
  else if (y->avl_balance == +2)
    {
      struct avl_node *x = y->avl_link[1];
      if (x->avl_balance == +1)
        {
          w = x;
          y->avl_link[1] = x->avl_link[0];
          x->avl_link[0] = y;

          y->avl_cnode[1] = x->avl_cnode[0]; /* add code by Yen-Chieh */
          x->avl_cnode[0] = y->avl_cnode[0] + y->avl_cnt + y->avl_cnode[1]; /* add code by Yen-Chieh */

          y->avl_sum[1] = x->avl_sum[0];
          x->avl_sum[0] = y->avl_sum[0] + (long long int)y->avl_cnt * y->avl_data + y->avl_sum[1]; /* add code by Yen-Chieh */

          x->avl_balance = y->avl_balance = 0;
        }
      else
        {
          assert (x->avl_balance == -1);
          w = x->avl_link[0];

          tmp1 = w->avl_cnode[0]; /* add code by Yen-Chieh */
          tmp2 = w->avl_cnode[1]; /* add code by Yen-Chieh */

          t1 = w->avl_sum[0]; /* add code by Yen-Chieh */
          t2 = w->avl_sum[1]; /* add code by Yen-Chieh */

          x->avl_link[0] = w->avl_link[1];
          w->avl_link[1] = x;
          y->avl_link[1] = w->avl_link[0];
          w->avl_link[0] = y;

          y->avl_cnode[1] = tmp1; /* add code by Yen-Chieh */
          x->avl_cnode[0] = tmp2; /* add code by Yen-Chieh */

          y->avl_sum[1] = t1; /* add code by Yen-Chieh */
          x->avl_sum[0] = t2; /* add code by Yen-Chieh */

          w->avl_cnode[0] = y->avl_cnode[0] + y->avl_cnt + y->avl_cnode[1]; /* add code by Yen-Chieh */
          w->avl_cnode[1] = x->avl_cnode[0] + x->avl_cnt + x->avl_cnode[1]; /* add code by Yen-Chieh */

          w->avl_sum[0] = y->avl_sum[0] + (long long int)y->avl_cnt * y->avl_data + y->avl_sum[1]; /* add code by Yen-Chieh */
          w->avl_sum[1] = x->avl_sum[0] + (long long int)x->avl_cnt * x->avl_data + x->avl_sum[1]; /* add code by Yen-Chieh */

          if (w->avl_balance == +1)
            x->avl_balance = 0, y->avl_balance = -1;
          else if (w->avl_balance == 0)
            x->avl_balance = y->avl_balance = 0;
          else /* |w->avl_balance == -1| */
            x->avl_balance = +1, y->avl_balance = 0;
          w->avl_balance = 0;
        }
    }
  else
    return &n->avl_data;

  tmp3 = (y != z->avl_link[0]); /* add code by Yen-Chieh */

  z->avl_link[tmp3] = w;

  /* add code by Yen-Chieh */
  if (w != NULL)
    {
      z->avl_cnode[tmp3] = w->avl_cnode[0] + w->avl_cnt + w->avl_cnode[1];
      z->avl_sum[tmp3] = w->avl_sum[0] + (long long int)w->avl_cnt * w->avl_data + w->avl_sum[1];
    }
  else
    {
      z->avl_cnode[tmp3] = 0;
      z->avl_sum[tmp3] = 0ll;
    }

  tree->avl_generation++;
  return &n->avl_data;
}

/* Allocates |size| bytes of space using |malloc()|.
   Returns a null pointer if allocation fails. */
void *
avl_malloc (struct libavl_allocator *allocator, size_t size)
{
  assert (allocator != NULL && size > 0);
  return malloc (size);
}

/* Frees |block|. */
void
avl_free (struct libavl_allocator *allocator, void *block)
{
  assert (allocator != NULL && block != NULL);
  free (block);
}

/* Default memory allocator that uses |malloc()| and |free()|. */
struct libavl_allocator avl_allocator_default =
  {
    avl_malloc,
    avl_free
  };

#undef NDEBUG
#include <assert.h>
