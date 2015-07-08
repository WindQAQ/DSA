#ifndef AVL_H
#define AVL_H 1

#include <stddef.h>

/* Function types. */
typedef int avl_comparison_func (const void *avl_a, const void *avl_b,
                                 void *avl_param);
typedef void avl_item_func (void *avl_item, void *avl_param);
typedef void *avl_copy_func (void *avl_item, void *avl_param);

#ifndef LIBAVL_ALLOCATOR
#define LIBAVL_ALLOCATOR
/* Memory allocator. */
struct libavl_allocator
  {
    void *(*libavl_malloc) (struct libavl_allocator *, size_t libavl_size);
    void (*libavl_free) (struct libavl_allocator *, void *libavl_block);
  };
#endif

/* Default memory allocator. */
extern struct libavl_allocator avl_allocator_default;
void *avl_malloc (struct libavl_allocator *, size_t);
void avl_free (struct libavl_allocator *, void *);

/* Maximum AVL height. */
#ifndef AVL_MAX_HEIGHT
#define AVL_MAX_HEIGHT 32
#endif

/* Tree data structure. */
struct avl_table
  {
    struct avl_node *avl_root;          /* Tree's root. */
    avl_comparison_func *avl_compare;   /* Comparison function. */
    void *avl_param;                    /* Extra argument to |avl_compare|. */
    struct libavl_allocator *avl_alloc; /* Memory allocator. */
    size_t avl_count;                   /* Number of items in tree. */
    unsigned long avl_generation;       /* Generation number. */
  };

/* An AVL tree node. */
struct avl_node
  {
    struct avl_node *avl_link[2];  /* Subtrees. */
    int  avl_cnt;
    int  avl_cnode[2];             /* count node */
    long long int  avl_sum[2];     /* sum of the key */
    int  avl_data;                 /* int data. */
    signed char avl_balance;       /* Balance factor. */
  };

/* AVL traverser structure. */
struct avl_traverser
  {
    struct avl_table *avl_table;        /* Tree being traversed. */
    struct avl_node *avl_node;          /* Current node in tree. */
    struct avl_node *avl_stack[AVL_MAX_HEIGHT];
                                        /* All the nodes above |avl_node|. */
    size_t avl_height;                  /* Number of nodes in |avl_parent|. */
    unsigned long avl_generation;       /* Generation number. */
  };

/* Table functions. */
struct avl_table *avl_create (avl_comparison_func *, void *,
                              struct libavl_allocator *);
int *avl_probe (struct avl_table *, int);
#define avl_count(table) ((size_t) (table)->avl_count)
#endif /* avl.h */
