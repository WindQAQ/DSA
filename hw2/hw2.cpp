#include <vector>
#include <cstdio>
#include <assert.h>
#include <set>
#include <utility>
#include <algorithm>
#include <cstring>
#include "ad.h"

#define get 0
#define clicked 1
#define impressed 2
#define profit 3
#define quit 4

using namespace std;

bool cmp(const Adproperty& L, const Adproperty& R)
{
	if(L.adid < R.adid)
		return true;
	return false;
}

bool cmp4profit(const Cl& L, const Cl& R)
{
	if(L.userid < R.userid)
		return true;
	return false;
}

typedef vector <Adproperty> Myvector;
typedef vector <Cl> Vector4Profit;

void cmdget(Myvector *data)
{
	int u, a, q, p, d;
	int sum_click = 0, sum_impression = 0;
	scanf("%d%d%d%u%u", &u, &a, &q, &p, &d);

	printf("********************\n");

	for(Myvector::iterator it = data[u].begin(); it != data[u].end(); it++)
		if((it->adid == a) && (it->queryid == q) && (it->position == p) && (it->depth == d)){
			sum_click += it->click;
			sum_impression += it->impression;
		}

	printf("%d %d\n", sum_click, sum_impression);

	printf("********************\n");

	return;
}

void cmdclicked(Myvector *data)
{
	int u;
	set <pair <int, int> > s;
	scanf("%d", &u);
	
	printf("********************\n");

	for(Myvector::iterator it = data[u].begin(); it != data[u].end(); it++)
		if(it->click > 0)
			s.insert(make_pair(it->adid, it->queryid));

	for(set <pair <int, int> >::iterator it = s.begin(); it != s.end(); it++)
		printf("%d %d\n", it->first, it->second);

	printf("********************\n");

	return;
}

void cmdimpressed(Myvector *data)
{
	int u1, u2;
	int id;
	int i = 0, j = 0;
	PartAd p;
	set <PartAd> s;
	scanf("%d%d", &u1, &u2);

	printf("********************\n");

	while((i < data[u1].size()) && (j < data[u2].size())){
		if(data[u1][i].adid < data[u2][j].adid)
			i++;
		if(data[u1][i].adid > data[u2][j].adid)
			j++;
		if(data[u1][i].adid == data[u2][j].adid){
			id = data[u1][i].adid;
			printf("%d\n", data[u1][i].adid);
			for(; i < data[u1].size() && data[u1][i].adid == id; i++){
				p.url = data[u1][i].url;
				p.keywordid = data[u1][i].keywordid;
				p.titleid = data[u1][i].titleid;
				p.advertiserid = data[u1][i].advertiserid;
				p.descriptionid = data[u1][i].descriptionid;
				s.insert(p);
			}
			for(; j < data[u2].size() && data[u2][j].adid == id; j++){
				p.url = data[u2][j].url;
				p.keywordid = data[u2][j].keywordid;
				p.titleid = data[u2][j].titleid;
				p.advertiserid = data[u2][j].advertiserid;
				p.descriptionid = data[u2][j].descriptionid;
				s.insert(p);
			}
			for(set <PartAd>::iterator it = s.begin(); it != s.end(); it++)
				printf("\t%llu %d %d %d %d\n", it->url, it->advertiserid, it->keywordid, it->titleid, it->descriptionid);
			s.clear();
		}
	}
	printf("********************\n");
	
	return;
}

void cmdprofit(Vector4Profit *CTI)
{
	double theta;
	int a, up;
	int click = 0, impression = 0;
	scanf("%d%lf", &a, &theta);
	if(!CTI[a].empty())
		up = CTI[a][0].userid;

	printf("********************\n");

	for(int i = 0; i < CTI[a].size(); i++){
		if(up != CTI[a][i].userid){
			if(((double)click / (double)impression) >= theta)
				printf("%d\n", up);
			click = 0;
			impression = 0;
		}

		click += CTI[a][i].click;
		impression += CTI[a][i].impression;
		up = CTI[a][i].userid;

		if(i == CTI[a].size() - 1)
			if(((double)click / (double)impression) >= theta)
				printf("%d\n", up);
	}

	printf("********************\n");
	
	return;
}

int findindex(char* s)
{
	const char *command[5] = {"get", "clicked", "impressed", "profit", "quit"};
	for(int i = 0; i < 5; i++)
		if(strcmp(s, command[i]) == 0)
			return i;
	return quit;
}

int main(int argc, char *argv[])
{
	int userid;
	Adproperty A;
	Cl c;
	char command[20];
	FILE *fin = fopen(argv[1], "r");
	Myvector* data = new Myvector [23907635];
	Vector4Profit * CTI = new Vector4Profit [22238278];
	assert(fin != NULL);

	while(fscanf(fin, "%d%d%llu%d%d%d%d%d%d%d%d%d", &A.click, &A.impression, &A.url, &A.adid,
		  &A.advertiserid, &A.depth, &A.position, &A.queryid, &A.keywordid, &A.titleid, &A.descriptionid, &userid) != EOF){
		c.userid = userid;
		c.click = A.click;
		c.impression = A.impression;
		data[userid].push_back(A);
		CTI[A.adid].push_back(c);
	}

	for(int i = 0; i < 23907635; i++)
		sort(data[i].begin(), data[i].end(), cmp);
	for(int i = 0; i < 22238278; i++)
		sort(CTI[i].begin(), CTI[i].end(), cmp4profit);

	fclose(fin);
	scanf("%s", command);

	while(findindex(command) != 4){
		switch(findindex(command)){
			case get:
				cmdget(data);
				break;
			case clicked:
				cmdclicked(data);
				break;
			case impressed:
				cmdimpressed(data);
				break;
			case profit:
				cmdprofit(CTI);
				break;
			case quit:
				break;
		}
		if(findindex(command) != 4)
			scanf("%s", command);
		else break;
	}

	return 0;
}
