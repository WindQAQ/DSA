#include <string>

#define MAX_WIDTH 62
#define MAX_LENGTH 105

typedef struct trienode{

	struct trienode* next[MAX_WIDTH];
	int index;

	trienode()
	{
		for(int i = 0; i < MAX_WIDTH; i++)
			next[i] = nullptr;
		index = -1;
	}

} Trienode;