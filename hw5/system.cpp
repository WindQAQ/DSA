#include <cstdio>
#include <cstring>
#include "binomial_heap.h"

#define MAX 10

class id_p{
public:
	int id, p;
	id_p(){};
	id_p(int id_in, int p_in){
		id = id_in;
		p = p_in;
	}
	bool operator>(const id_p& in) const{
		if(p > in.p)
			return true;
		else if(p == in.p && id < in.id)
			return true;
		return false;
	}
};

int main()
{
	int c, w;
	char command[MAX];
	scanf("%d %d", &c, &w);
	BinomialHeap <id_p> *computer = new BinomialHeap <id_p> [c];
	while(scanf("%s", command) != EOF){
		if(strcmp(command, "assign") == 0){
			int cm;
			id_p temp;
			scanf("%d %d %d", &cm, &temp.id, &temp.p);
			computer[cm].insert(temp);
			printf("There are %d tasks on computer %d.\n", computer[cm].size, cm);
		}
		else if(strcmp(command, "execute") == 0){
			int cm;
			scanf("%d", &cm);
			id_p temp = computer[cm].pop();
			printf("Computer %d executed task %d.\n", cm, temp.id);
			while(computer[cm].size != 0 && (temp.p == computer[cm].max().p)){
				temp = computer[cm].pop();
				printf("Computer %d executed task %d.\n", cm, temp.id);
			}
		}
		else{
			int cm1, cm2;
			scanf("%d %d", &cm1, &cm2);
			if(computer[cm2].size >= w){
				computer[cm1].merge(computer[cm2]);
				printf("The largest priority number is now %d on %d.\n", computer[cm1].max().p, cm1);
			}
			else{
				printf("Merging request failed.\n");
			}
		}
	}
	delete computer;
	return 0;
}
