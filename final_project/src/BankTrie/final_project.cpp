#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>
#include "trie.h"
#include "md5.h"

#define MAX_TABLE 62
#define MAX_COMMAND 20
#define MAX 1000000

const static char table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
							 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
							 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
							 'U', 'V', 'W', 'X', 'Y', 'Z',
							 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
							 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
							 'u', 'v', 'w', 'x', 'y', 'z'};

typedef struct Account{
	unsigned int money;
	std::string psd;
} Account;

typedef struct log{
	char type;
	int index;
	char id[MAX_LENGTH];
	unsigned int cash;
	int time;
	log(char _type, int _index, char* _id, unsigned int _cash, int _time):
	type(_type), index(_index), cash(_cash), time(_time){strcpy(id, _id);}
} Log;

using namespace std;
Account *Banksystem = new Account[MAX];
unordered_map<int, char*> totalID;
vector <Log> *history = new vector <Log> [MAX];

typedef unordered_map<int, char*> mymap;
typedef unordered_map<int, char*>::iterator mapiterator;

void insert_trie(Trienode*, const char*, int&); // add an string to trie and, there is a unique index to each string
int lookup_trie(Trienode*, const char*); // check whether the string exists
int change_index(Trienode*, const char*); // set index to -1
inline int score(const char*, const char*); // compute the score of two strings
inline void swap(pair <char*, int>&, pair <char*, int>&); // swap two pair<char*, int>
int wildcmp(const char*, const char*); // check whether the string is match to the wildcard
void recommend_create(Trienode*, const char*); // find 10 free ID in "create" command
void recommend_transfer(mapiterator, const mapiterator, int, const char*); // find 10 exsiting ID
void change_ID(vector <Log>&, int, const char*, int); // change the ID in "merge" command
void Union(Trienode*, int&, int&); // merge two history in "merge" command
void isMatch(mapiterator, const mapiterator, const char*, const char*); // find all ID which is match to wildcard

// wildcmp algorithm from: 
// http://www.codeproject.com/Articles/1088/Wildcard-string-compare-globbing
int wildcmp(const char* wild, const char* str)
{

	int i = 0, j = 0;
	int mp, cp;

	while((str[i]) && (wild[j] != '*')){
	    if((wild[j] != str[i]) && (wild[j] != '?'))
	        return 0;
	    i++;
	    j++;
	}

	while(str[i]){
	    if(wild[j] == '*'){
	        if(!wild[++j])  //if * is the last char
	            return 1;  
	      mp = j;
	      cp = i+1;
	    } 
	    else if((wild[j] == str[i]) || (wild[j] == '?')){
	        i++;
	        j++;
	    } 
	    else{
	    	i = cp++;
	    	j = mp; // assign first then increase cp by 1
	    }
	}

	while(wild[j] == '*')
	    j++;

	return !wild[j];
}

void recommend_create(Trienode* root, const char* id)
{
	char test[MAX_LENGTH];
	strcpy(test, id);
	int firstlastcharindex = 0;
	int secondlastcharindex = 0;
	int thirdlastcharindex = 0;
	int id_length = strlen(id);
	bool found = false;
	int count = 0;

	if(id_length > 0){
		while(table[firstlastcharindex] != id[id_length- 1])
			firstlastcharindex++;
	}
	if(id_length > 1){
		while(table[secondlastcharindex] != id[id_length - 2])
			secondlastcharindex++;
	}
	if(id_length > 2){
		while(table[thirdlastcharindex] != id[id_length - 3])
			thirdlastcharindex++;
	}

	//score 1 : delete last char
	int test_length = strlen(test);
	if(test_length > 1){
		test[test_length-1] = '\0';

		if(lookup_trie(root, test) == -1 && test_length != 0){
			printf("%s", test);
			found = true;
			count++;
		}
	}

	//score 1 : change last char with a char prior to the original one
	strcpy(test, id);
	test_length = strlen(test);
	
	for(int i = 0; count < 10 && table[i] != id[id_length - 1] && i < MAX_TABLE; i++){
		test[test_length-1] = table[i];

		if(lookup_trie(root, test) == -1){
			if(found)
				printf(",%s", test);
			else{
				printf("%s", test);
			}
			found = true;
			count++;
		}
	}

	if(count == 10)
		return;
	//score 1 : add one char
	strcpy(test, id);
	test_length = strlen(test);
	
	for(int i = 0; count < 10 && i < MAX_TABLE; i++){
		test[test_length] = table[i];
		test[test_length+1] = '\0';

		if(lookup_trie(root, test) == -1){
			if(found)
				printf(",%s", test);
			else{
				printf("%s", test);
			}
			found = true;
			count++;
		}
	}	

	if(count == 10)
		return;
	//score 1 : change last char with a char with lower priority with original one 
	strcpy(test, id);
	test_length = strlen(test);
	
	for(int i = firstlastcharindex + 1; count < 10 && i < MAX_TABLE; i++){
		test[test_length-1] = table[i];

		if(lookup_trie(root, test) == -1){
			if(found)
				printf(",%s", test);
			else{
				printf("%s", test);
			}
			found = true;
			count++;
		}
	}

	if(count == 10)
		return;

	//score 2: delete last char and change the last char of the new string
	//score 2: change the second-last char
	strcpy(test, id);
	test_length = strlen(test);

	if(test_length >= 2){

		for(int i = 0; count < 10 && i < secondlastcharindex; i++){
			//remove the last char and replace the new string's last char
			test[test_length-1] = '\0';
			test[test_length-2] = table[i];

			if(lookup_trie(root, test) == -1){
				if(found)
					printf(",%s", test);
				else{
					printf("%s", test);
				}
				found = true;
				count++;
			}

			test[test_length-1] = id[id_length-1];
			test[test_length] = '\0';

			if(lookup_trie(root, test) == -1){
				if(found)
					printf(",%s", test);
				else{
					printf("%s", test);
				}
				found = true;
				count++;
			}
		}

		if(count == 10)
			return;
	}

	//score 2: change the last char of the string and add a char
	strcpy(test, id);
	test_length = strlen(test);

	for(int i = 0; count < 10 && i < firstlastcharindex; i++){
		test[test_length-1] = table[i];
		for(int j = 0; count < 10 && j < MAX_TABLE; j++){
			test[test_length] = table[j];
			test[test_length+1] = '\0';

			if(lookup_trie(root, test) == -1){
				if(found)
					printf(",%s", test);
				else{
					printf("%s", test);
				}
				found = true;
				count++;
			}

			test[test_length] = '\0';
		}
	}

	if(count == 10)
		return;
	//score 2: change the last char of the string and add a char(priority after original)
	strcpy(test, id);
	test_length = strlen(test);

	for(int i = firstlastcharindex + 1; count < 10 && i < MAX_TABLE; i++){
		test[test_length - 1] = table[i];
		for(int j = 0; count < 10 && j < MAX_TABLE; j++){
			test[test_length] = table[j];
			test[test_length+1] = '\0';
			if(lookup_trie(root, test) == -1){
				if(found)
					printf(",%s", test);
				else{
					printf("%s", test);
				}
				found = true;
				count++;
			}
			test[test_length] = table[j];
		}
	}

	if(count == 10)
		return;

	//score 2: delete last char and change the last char of the new string(prioirty after original)
	//score 2: change the second-last char(prioirty after original)
	strcpy(test, id);
	test_length = strlen(test);

	if(test_length >= 2){

		for(int i = secondlastcharindex + 1; count < 10 && i < MAX_TABLE; i++){
			//remove the last char and replace the new string's last char
			test[test_length-1] = '\0';
			
			test[test_length-2] = table[i];

			if(lookup_trie(root, test) == -1){
				if(found)
					printf(",%s", test);
				else{
					printf("%s", test);
				}
				found = true;
				count++;
			}

			test[test_length-1] = id[id_length-1];
			test[test_length] = '\0';

			if(lookup_trie(root, test) == -1){
				if(found)
					printf(",%s", test);
				else{
					printf("%s", test);
				}
				found = true;
				count++;
			}
		}

		if(count == 10)
			return;
	}

	//score 3: delete the last char and change the second-last char of new string 
	//score 3: change the third-last char
	
	strcpy(test, id);
	test_length = strlen(test);
	
	if(test_length >= 3){

		test[test_length-1] = '\0';

		for(int i = 0; count < 10 && i < thirdlastcharindex; i++){
			
			//remove the last char and replace the new string's last char
			test[test_length - 3] = table[i];

			if(lookup_trie(root, test) == -1){
				if(found)
					printf(",%s", test);
				else{
					printf("%s", test);
				}
				found = true;
				count++;
			}

			test[test_length-1] = id[id_length-1];
			test[test_length] = '\0';

			if(lookup_trie(root, test) == -1){
				if(found)
					printf(",%s", test);
				else{
					printf("%s", test);
				}
				found = true;
				count++;
			}

			test[test_length-1] = '\0';
		}

		if(count == 10)
			return;
	}
	//score 3: delete last two char
	strcpy(test, id);
	test_length = strlen(test);

	if(test_length > 2){
		test[test_length-2] = '\0';

		if(lookup_trie(root, test) == -1){
			if(found)
				printf(",%s", test);
			else{
				printf("%s", test);
			}
			found = true;
			count++;
		}

		if(count == 10)
			return;
	}
	//score 3: change the last two char
	strcpy(test, id);
	test_length = strlen(test);

	if(test_length > 1){
		for(int i = 0; count < 10 && i < secondlastcharindex; i++){
			test[test_length - 2] = table[i];
			
			for(int j = 0; count < 10 && j < MAX_TABLE; j++){
				if(j != firstlastcharindex){
					test[test_length - 1] = table[j];
					
					if(lookup_trie(root, test) == -1){
						if(found)
							printf(",%s", test);
						else{
							printf("%s", test);
						}
						found = true;
						count++;
					}
				}
			}
		}

		if(count == 10)
			return;
	}
	//score 3: add two char
	strcpy(test, id);
	test_length = strlen(test);

	for(int i = 0; count < 10 && i < MAX_TABLE; i++){
		test[test_length] = table[i];
		for(int j = 0; count < 10 && j < MAX_TABLE; j++){
			test[test_length+1] = table[j];
			test[test_length+2] = '\0';
			if(lookup_trie(root, test) == -1){
				if(found)
					printf(",%s", test);
				else{
					printf("%s", test);
				}
				found = true;
				count++;
			}
		}
	}

	if(count == 10)
		return;

	//score 3: change last two char(priority after original)
	strcpy(test, id);
	test_length = strlen(test);

	if(test_length > 1){
		for(int i = secondlastcharindex + 1; count < 10 && i < MAX_TABLE; i++){
			test[test_length-2] = table[i];
			
			for(int j = 0; count < 10 && j < MAX_TABLE; j++){
				if(j != firstlastcharindex){
					test[test_length-1] = table[j];
					
					if(lookup_trie(root, test) == -1){
						if(found)
							printf(",%s", test);
						else{
							printf("%s", test);
						}
						found = true;
						count++;
					}
				}
			}
		}

		if(count < 10)
			return;
	}
	//score 3: change the third-last char and delete the last char(priority after original)
	//score 3: change the third-last char(priority after original)
	strcpy(test, id);
	test_length = strlen(test);
	
	if(test_length > 2){
		for(int i = thirdlastcharindex + 1; count < 10 && i < MAX_TABLE; i++){

			test[test_length - 3] = table[i];
			test[test_length-1] = '\0';
		
			if(lookup_trie(root, test) == -1){
				if(found)
					printf(",%s", test);
				else{
					printf("%s", test);
				}
				found = true;
				count++;
			}

			test[test_length-1] = id[id_length-1];
			test[test_length] = '\0';

			if(lookup_trie(root, test) == -1){
				if(found)
					printf(",%s", test);
				else{
					printf("%s", test);
				}
				found = true;
				count++;
			}	
		}
		if(count == 10)
			return;
	}
}

void recommend_transfer(mapiterator it, const mapiterator end, int mapsize, char* ID2)
{
	vector<pair<char*, int> >recommend;
	recommend.reserve(mapsize);

	for(; it != end; it++)
		recommend.push_back(make_pair(it->second, score(it->second, ID2)));
	// apply selection sort to find the top 10
	int min;
	for(int i = 0; i < 10 && i < mapsize-1; i++){
		min = i;
		for(int j = i+1; j < mapsize; j++)
			if(recommend[min].second > recommend[j].second || 
			  (recommend[min].second == recommend[j].second && strcmp(recommend[min].first, recommend[j].first) > 0))
				min = j;
		if(min != i)
			swap(recommend[min], recommend[i]);
	}
	if(mapsize != 0)
		printf("%s", recommend[0].first);
	for(int i = 1; i < 10 && i < mapsize; i++)
		printf(",%s", recommend[i].first);
	printf("\n");
}

void change_ID(vector <Log>& a, int time, const char* id, int index1)
{
	int size = a.size();

	if(size == 0)
		return;

	int left = 0;
	int right = size - 1;
	int middle = (left + right) / 2;

	if(a[0].time == time){
		a[0].index = index1;
		strcpy(a[0].id, id);
		if(1 < size && a[1].time == time){
			a[1].index = index1;
			strcpy(a[1].id, id);
		}
		return;
	}
	if(a[right].time == time){
		a[right].index = index1;
		strcpy(a[right].id, id);
		if(right-1 >= 0 && a[right-1].time == time){
			a[right-1].index = index1;
			strcpy(a[right-1].id, id);
		}
		return;
	}

	while(a[middle].time != time && left < right){
		if(a[middle].time > time)
			right = middle - 1;
		else if(a[middle].time < time)
			left = middle + 1;
		middle = (left + right) / 2;
	}
	
	if(a[middle].time == time){
		a[middle].index = index1;
		strcpy(a[middle].id, id);
		if(middle+1 < size && a[middle+1].time == time){
			a[middle+1].index = index1;
			strcpy(a[middle+1].id, id);
		}
		if(middle-1 >= 0 && a[middle-1].time == time){
			a[middle-1].index = index1;
			strcpy(a[middle-1].id, id);
		}
	}
}

void Union(Trienode* root, int& index1, int& index2)
{
	int size1 = history[index1].size(), size2 = history[index2].size();
	vector <Log> result;
	int i = 0, j = 0;
	int index;
	char id[MAX_LENGTH];
	strcpy(id, totalID[index1]);

	while(i < size1 && j < size2){
		if(history[index1][i].time < history[index2][j].time){
			result.push_back(history[index1][i]);
			i++;
		}
		else if(history[index1][i].time > history[index2][j].time){
			if(totalID.find(history[index2][j].index) != totalID.end()){
				index = lookup_trie(root, totalID[history[index2][j].index]);
				if(index != -1)
					change_ID(history[index], history[index2][j].time, id, index1);
			}
			result.push_back(history[index2][j]);
			j++;
		}
		else if(history[index1][i].time == history[index2][j].time){
			if(totalID.find(history[index2][j].index) != totalID.end()){
				index = lookup_trie(root, totalID[history[index2][j].index]);
				if(index != -1)
					change_ID(history[index], history[index2][j].time, id, index1);
			}
			result.push_back(history[index1][i]);
			result.push_back(history[index2][j]);
			i++;
			j++;
		}
	}

	while(i < size1){
		result.push_back(history[index1][i]);
		i++;
	}

	while(j < size2){
		if(totalID.find(history[index2][j].index) != totalID.end()){
			index = lookup_trie(root, totalID[history[index2][j].index]);
			if(index != -1)
				change_ID(history[index], history[index2][j].time, id, index1);
		}
		result.push_back(history[index2][j]);
		j++;
	}

	history[index1] = result;
}

void isMatch(mapiterator it, const mapiterator end, const char* wildcard, const char* last)
{
	vector <char*> wild;

	for(; it != end; it++)
		if(wildcmp(wildcard, it->second) && strcmp(it->second, last) != 0)
			wild.push_back(it->second);

	sort(wild.begin(), wild.end(), 
		[](const char* a, const char* b){
			int key = strcmp(a, b);
			if(key < 0) return true;
			return false;
		});

	int size = wild.size();

	if(size != 0)
		printf("%s", wild[0]);

	for(int i = 1; i < size; i++)
		printf(",%s", wild[i]);

	printf("\n");
}

inline int score(const char* a, const char* b)
{
	int len_a = strlen(a), len_b = strlen(b);
	int del = fabs(len_a-len_b);
	int len = min(len_a, len_b);
	int first = (del+1)*del/2;
	int second = 0;

	for(int i = 0; i < len; i++)
		if(a[i] != b[i])
			second += (len - i);

	return (first+second);
}

inline void swap(pair <char*, int>& a, pair <char*, int>& b)
{
	pair <char*, int> temp = a;
	a = b;
	b = temp;
}

int main()
{
	Trienode* root = new Trienode();
	char command[MAX_COMMAND];
	char ID1[MAX_LENGTH], password1[MAX_LENGTH];
	char ID2[MAX_LENGTH], password2[MAX_LENGTH];
	char wildcard[MAX_LENGTH];
	unsigned int num;
	int count = 0, time = 0;
	int index, curindex = -1;
	char curID[MAX_LENGTH];
	
	while(scanf("%s", command) != EOF){
		if(strcmp(command, "login") == 0){
			scanf("%s%s", ID1, password1);
			index = lookup_trie(root, ID1);
			if(index == -1)
				printf("ID %s not found\n", ID1);
			else if(Banksystem[index].psd != md5(password1))
				printf("wrong password\n");
			else{
				curindex = index;
				strcpy(curID, ID1);
				printf("success\n");
			}
		}
		if(strcmp(command, "create") == 0){
			scanf("%s%s", ID1, password1);
			index = lookup_trie(root, ID1);
			if(index != -1){
				printf("ID %s exists, ", ID1);
				recommend_create(root, ID1);
				printf("\n");
			}
			else{
				insert_trie(root, ID1, count);
				Banksystem[count].psd = md5(password1);
				Banksystem[count].money = 0;
				char *str = new char[MAX_LENGTH];
				strcpy(str, ID1);
				totalID.insert(make_pair(count, str));
				printf("success\n");
				count++;
			}
		}
		if(strcmp(command, "delete") == 0){
			scanf("%s%s", ID1, password1);
			index = lookup_trie(root, ID1);
			if(index == -1)
				printf("ID %s not found\n", ID1);
			else if(Banksystem[index].psd != md5(password1))
				printf("wrong password\n");
			else{
				change_index(root, ID1);
				delete totalID[index];
				totalID.erase(index);
				printf("success\n");
			}
		}
		if(strcmp(command, "merge") == 0){
			scanf("%s%s%s%s", ID1, password1, ID2, password2);
			int index1 = lookup_trie(root, ID1);
			int index2 = lookup_trie(root, ID2);
			if(index1 == -1)
				printf("ID %s not found\n", ID1);
			else if(index2 == -1)
				printf("ID %s not found\n", ID2);
			else if(Banksystem[index1].psd != md5(password1))
				printf("wrong password1\n");
			else if(Banksystem[index2].psd != md5(password2))
				printf("wrong password2\n");
			else{
				Banksystem[index1].money += Banksystem[index2].money;
				printf("success, %s has %u dollars\n", ID1, Banksystem[index1].money);
				Union(root, index1, index2);
				change_index(root, ID2);
				delete totalID[index2];
				totalID.erase(index2);
			}
		}
		if(strcmp(command, "deposit") == 0){
			scanf("%u", &num);
			Banksystem[curindex].money += num;
			printf("success, %u dollars in current account\n", Banksystem[curindex].money); 
		}
		if(strcmp(command, "withdraw") == 0){
			scanf("%u", &num);
			if(Banksystem[curindex].money >= num){
				Banksystem[curindex].money -= num;
				printf("success, %u dollars left in current account\n", Banksystem[curindex].money); 
			}
			else
				printf("fail, %u dollars only in current account\n", Banksystem[curindex].money);
		}
		if(strcmp(command, "transfer") == 0){
			scanf("%s%u", ID2, &num);
			index = lookup_trie(root, ID2);
			if(index == -1){
				printf("ID %s not found, ", ID2);
				mapiterator it = totalID.begin(), end = totalID.end();
				int mapsize = totalID.size();
				recommend_transfer(it, end, mapsize, ID2);
			}
			else if(Banksystem[curindex].money < num)
				printf("fail, %u dollars only in current account\n", Banksystem[curindex].money);
			else{
				Banksystem[curindex].money -= num;
				Banksystem[index].money += num;
				history[curindex].push_back(Log('T', index, ID2, num, time));
				history[index].push_back(Log('F', curindex, curID, num, time));
				printf("success, %u dollars left in current account\n", Banksystem[curindex].money);
				time++;
			}
		}
		if(strcmp(command, "find") == 0){
			scanf("%s", wildcard);
			mapiterator it = totalID.begin(), end = totalID.end();
			isMatch(it, end, wildcard, curID);
		}
		if(strcmp(command, "search") == 0){
			scanf("%s", ID2);
			index = lookup_trie(root, ID2);
			int size = history[curindex].size();
			bool found = false;

			for(int i = 0; i < size; i++){
				if(strcmp(history[curindex][i].id, ID2) == 0){
					if(history[curindex][i].type == 'T')
						printf("To %s %u\n", ID2, history[curindex][i].cash);
					else
						printf("From %s %u\n", ID2, history[curindex][i].cash);
					found = true;
				}
			}

			if(!found)
				printf("no record\n");
		}
	}

	return 0;
}