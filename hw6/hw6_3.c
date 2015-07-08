#include <stdio.h>
#include <stdlib.h>
#include "avl_ntudsa.h"

#define MAX 100010

int int_compare(const void *pa, const void *pb, void *param)
{
	int a = *(const int *)pa;
	int b = *(const int *)pb;

	if(a < b) return -1;
	else if(a > b) return 1;
	else return 0;
}

void merge(struct avl_table *tree, struct avl_node *node)
{
	int i;
	if(node == NULL)
		return;
	for(i = 0; i < node->avl_cnt; i++)
		avl_probe(tree, node->avl_data);
	if(node->avl_link[0] != NULL || node->avl_link[1] != NULL){
		merge(tree, node->avl_link[0]);
		merge(tree ,node->avl_link[1]);
	}
	free(node);
}

int Find(int x, int *father)
{
	return (x == father[x])? x: (father[x] = Find(father[x], father));
}

void Union(int x, int y, int *father, int *rank, struct avl_table **tree)
{
	if(rank[x] > rank[y]){
		father[y] = x;
		merge(tree[x], tree[y]->avl_root);
	}
	else{
		father[x] = y;
		if(rank[x] == rank[y])
			rank[y]++;
		merge(tree[y], tree[x]->avl_root);
	}
}

void get_max_game(struct avl_node *node, long long int money, int *count)
{
	if(node == NULL)
		return;
	if(node->avl_sum[0] <= money){
		(*count) += node->avl_cnode[0];
		money -= node->avl_sum[0];
		int count_temp = (int)(money / node->avl_data);
		count_temp = (node->avl_cnt <= count_temp)? node->avl_cnt: count_temp;
		(*count) += count_temp;
		money -= (long long int)(node->avl_data) * count_temp;
		get_max_game(node->avl_link[1], money, count);
	}
	else
		get_max_game(node->avl_link[0], money, count);
}

int main()
{
	int n, m, i;

	scanf("%d%d", &n, &m);
	
	static struct avl_table *tree[MAX];
	static int father[MAX];
	static int rank[MAX];
	static int owner[MAX];
	long long int price;

	for(i = 0; i < n; i++){
		scanf("%lld", &price);
		tree[i] = avl_create(int_compare, NULL, NULL);
		avl_probe(tree[i], price);
		father[i] = i;
		owner[i] = i;
		rank[i] = 0;
	}

	int type, count;
	int now_game, want_game;
	int father_now, father_want;
	long long int money;

	for(i = 0; i < m; i++){
		scanf("%d", &type);
		if(type == 1){
			scanf("%d%d", &now_game, &want_game);
			father_now = Find(now_game-1, father);
			father_want = Find(want_game-1, father);
			if(father_now != father_want){
				owner[father_want] = owner[father_now];
				Union(father_now, father_want, father, rank, tree);
			}
		}
		else{
			count = 0;
			scanf("%d%lld", &want_game, &money);
			father_want = Find(want_game-1, father);
			get_max_game(tree[father_want]->avl_root, money, &count);
			printf("%d %d\n", owner[father_want]+1, count);
		}
	}
	return 0;
}