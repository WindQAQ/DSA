#include <cstdio>
#include <cstring>
#include "trie.h"

inline int hash(const char& c)
{
	if((int)c <= 57) // '0'~'9' => 0~9
		return (int)c - 48;
	if((int)c <= 90) // 'A'~'Z' => 10~35
		return (int)c - 55;
	if((int)c <= 122) // 'a'~'z' => 36~61
		return (int)c - 61;
}

inline char hash_back(const int& x)
{
	if(x >= 36)
		return char(x+61);
	if(x >= 10)
		return char(x+55);
	if(x >= 0)
		return char(x+48);
}

void insert_trie(Trienode* node, const char* str, int& c)
{
	int index = 0;
	int i = 0;

	while(str[i] != '\0'){
		index = hash(str[i]);
		if(node->next[index] == nullptr)
			node->next[index] = new Trienode();
		node = node->next[index];
		i++;
	}

	node->index = c;
	return;
}

int lookup_trie(Trienode* node, const char* str)
{
	int index = 0;
	int i = 0;

	while(str[i] != '\0'){
		index = hash(str[i]);
		if(node->next[index] == nullptr)
			return -1;
		node = node->next[index];
		i++;
	}

	return node->index;
}

int change_index(Trienode* node, const char* str)
{
	int index;
	int i = 0;

	while(str[i] != '\0'){
		index = hash(str[i]);
		node = node->next[index];
		i++;
	}

	node->index = -1;
}