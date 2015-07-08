#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "bst.h"
#include "rb.h"

#define MAX 200

void preorder_string_avl(const struct avl_node *node)
{
	if(node == NULL)
		return;
	printf("%s ", (char *)node->avl_data);
	if(node->avl_link[0] != NULL || node->avl_link[1] != NULL){
		putchar('(');
		preorder_string_avl(node->avl_link[0]);
		putchar(',');
		putchar(' ');
		preorder_string_avl(node->avl_link[1]);
		putchar(')');
	}
}

void preorder_string_bst(const struct bst_node *node)
{
	if(node == NULL)
		return;
	printf("%s ", (char *)node->bst_data);
	if(node->bst_link[0] != NULL || node->bst_link[1] != NULL){
		putchar('(');
		preorder_string_bst(node->bst_link[0]);
		putchar(',');
		putchar(' ');
		preorder_string_bst(node->bst_link[1]);
		putchar(')');
	}
}

void preorder_string_rb(const struct rb_node *node)
{
	if(node == NULL)
		return;
	printf("%s ", (char *)node->rb_data);
	if(node->rb_link[0] != NULL || node->rb_link[1] != NULL){
		putchar('(');
		preorder_string_rb(node->rb_link[0]);
		putchar(',');
		putchar(' ');
		preorder_string_rb(node->rb_link[1]);
		putchar(')');
	}
}

int str_compare(const void *pa, const void *pb, void *param)
{
	char *a = (char *)pa;
	char *b = (char *)pb;

	int key = strcmp(a, b);
	if(key < 0) return -1;
	else if(key > 0) return +1;
	else return 0;
}

int main()
{
	struct avl_table *tree_avl;
	struct bst_table *tree_bst;
	struct rb_table *tree_rb;
	tree_avl = avl_create(str_compare, NULL, NULL);
	tree_bst = bst_create(str_compare, NULL, NULL);
	tree_rb = rb_create(str_compare, NULL, NULL);

	char str[MAX];

	int i, len = 0;
	for(i = 0; i < 32; i++){
		fgets(str, MAX, stdin);
		len = strlen(str);
		if(str[len-1] == '\n')
			str[len-1] = '\0';
		char *element_avl = (char *)malloc(sizeof(char) * MAX);
		char *element_bst = (char *)malloc(sizeof(char) * MAX);
		char *element_rb = (char *)malloc(sizeof(char) * MAX);
		strcpy(element_avl, str);
		strcpy(element_bst, str);
		strcpy(element_rb, str);
		void **p1 = avl_probe(tree_avl, element_avl);
		void **p2 = bst_probe(tree_bst, element_bst);
		void **p3 = rb_probe(tree_rb, element_rb);
	}

	preorder_string_avl(tree_avl->avl_root);
	puts("");
	preorder_string_bst(tree_bst->bst_root);
	puts("");
	preorder_string_rb(tree_rb->rb_root);
	puts("");
	return 0;
}