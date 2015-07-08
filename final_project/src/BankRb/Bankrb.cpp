#include <iostream>
#include <string>
#include <cmath>
#include <utility>
#include "md5.h"
#include "Account.h"
extern "C"
{
#include "rb.h"
}

using namespace std;
bool check_find;

int score(const string& a, const string& b)
{
	int len_a = a.size(), len_b = b.size();
	int del = fabs(len_a-len_b);
	int len = min(len_a, len_b);
	int first = (del+1)*del/2;
	int second = 0;

	for(int i = 0; i < len; i++)
		if(a[i] != b[i])
			second += (len - i);

	return (first+second);
}

// wildcmp algorithm from: 
// http://www.codeproject.com/Articles/1088/Wildcard-string-compare-globbing
int wildcmp(const std::string& wild, const std::string& str)
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

void isMatch(const struct rb_node *node, const string& wild, const string& last)
{
	if(node == NULL)
		return;
	if(node->rb_link[0] != NULL)
		isMatch(node->rb_link[0], wild, last);
	if(wildcmp(wild, ((Account*)node->rb_data)->id) && ((Account*)node->rb_data)->id != last){
		if(!check_find){
			check_find = true;
			cout << ((Account*)node->rb_data)->id;
		}
		else
			cout << "," << ((Account*)node->rb_data)->id;
	}
	if(node->rb_link[1] != NULL)
		isMatch(node->rb_link[1], wild, last);
}

void traverse(const struct rb_node *node, vector <pair <string, int> >& recommend, const string& id)
{
	if(node == NULL)
		return;
	recommend.push_back(make_pair(((Account*)node->rb_data)->id, score(((Account*)node->rb_data)->id, id)));
	if(node->rb_link[0] != NULL || node->rb_link[1] != NULL){
		traverse(node->rb_link[0], recommend, id);
		traverse(node->rb_link[1], recommend, id);
	}
}

void recommend_transfer(vector <pair<string, int > >& recommend)
{
	int min, j, size = recommend.size();
	string temps;
	int tempi;

	for(int i = 0; i < 10 && i < size-1; i++){
		min = i;
		for(j = i+1; j < size; j++){
			if(recommend[min].second > recommend[j].second ||
			  (recommend[min].second == recommend[j].second && recommend[min].first.compare(recommend[j].first) > 0))
			min = j;
		}
		if(min != i){
			temps = recommend[min].first;
			tempi = recommend[min].second;
			recommend[min].first = recommend[i].first;
			recommend[min].second = recommend[i].second;
			recommend[i].first = temps;
			recommend[i].second = tempi;
		}
	}

	if(size != 0)
		cout << recommend[0].first;
	for(int i = 1; i < 10 && i < size; i++)
		cout << "," << recommend[i].first;
	cout << endl;
}

int account_cmp(const void* pa, const void* pb, void* param)
{
	Account a = *(const Account*)pa;
	Account b = *(const Account*)pb;

	int key = a.compare(b);
	if(key < 0) return -1;
	else if(key > 0) return 1;
	else return 0;
}

void recommend_create(struct rb_table *, const string&);

int main()
{
	ios::sync_with_stdio(false);
	string command;
	string ID1, ID2, psd1, psd2;
	string curID;
	string wildcard;
	Account temp, temp2;
	Account *curAccount, *toAccount, *m1, *m2;
	void *ptr, *ptr2;
	int money;
	int time = 0, count = 0;
	struct rb_table *banksystem;
	banksystem = rb_create(account_cmp, NULL, NULL);

	while(cin >> command){
		if(command == "login"){
			cin >> ID1 >> psd1;
			temp.id = ID1;
			ptr = rb_find(banksystem, &temp);
			if(ptr == NULL)
				cout << "ID " << ID1 << " not found" << endl;
			else{
				temp = *(Account*)ptr;
				if(!temp.check_psd(md5(psd1)))
					cout << "wrong password" << endl;
				else{
					curAccount = (Account*)ptr;
					curID = ID1;
					cout << "success" << endl;
				}
			}
		}
		else if(command == "create"){
			cin >> ID1 >> psd1;
			temp.id = ID1;
			ptr = rb_find(banksystem, &temp);
			if(ptr != NULL){
				cout << "ID " << ID1 << " exists, ";
				recommend_create(banksystem, ID1);
			}
			else{
				Account *new_acc = new Account(ID1, md5(psd1));
				rb_probe(banksystem, new_acc);
				count++;
				cout << "success" << endl;
			}
		}
		else if(command == "delete"){
			cin >> ID1 >> psd1;
			temp.id = ID1;
			ptr = rb_find(banksystem, &temp);
			if(ptr == NULL)
				cout << "ID " << ID1 << " not found" << endl;
			else{
				temp = *(Account*)ptr;
				if(!temp.check_psd(md5(psd1)))
					cout << "wrong password" << endl;
				else{
					((Account*)ptr)->hist_null();
					delete (Account*)rb_delete(banksystem, &temp);
					count--;
					cout << "success" << endl;
				}
			}
		}
		else if(command == "merge"){
			cin >> ID1 >> psd1 >> ID2 >> psd2;
			temp.id = ID1;
			temp2.id = ID2;
			ptr = rb_find(banksystem, &temp);
			ptr2 = rb_find(banksystem, &temp2);
			if(ptr == NULL)
				cout << "ID " << ID1 << " not found" << endl;
			else if(ptr2 == NULL)
				cout << "ID " << ID2 << " not found" << endl;
			else if(ptr != NULL && ptr2 != NULL){
				m1 = (Account*)ptr;
				m2 = (Account*)ptr2;
				if(!m1->check_psd(md5(psd1)))
					cout << "wrong password1" << endl;
				else if(!m2->check_psd(md5(psd2)))
					cout << "wrong password2" << endl;
				else{
					m1->merge_hist(m2);
					delete (Account*)rb_delete(banksystem, &temp2);
					count--;
				}
			}
		}
		else if(command == "deposit"){
			cin >> money;
			curAccount->deposit(money);
		}
		else if(command == "withdraw"){
			cin >> money;
			curAccount->withdraw(money);
		}
		else if(command == "transfer"){
			cin >> ID1 >> money;
			temp.id = ID1;
			ptr = rb_find(banksystem, &temp);
			if(ptr == NULL){
				cout << "ID " << ID1 << " not found, ";
				vector <pair<string, int> > recommend;
				recommend.reserve(count);
				traverse(banksystem->rb_root, recommend, ID1);
				recommend_transfer(recommend);
			}
			else{
				toAccount = (Account*)ptr;
				curAccount->transfer(toAccount, money, time);
			}
			time++;
		}
		else if(command == "find"){
			check_find = false;
			cin >> wildcard;
			isMatch(banksystem->rb_root, wildcard, curID);
			cout << endl;
		}
		else if(command == "search"){
			cin >> ID1;
			curAccount->search(ID1);
		}
	}

	return 0;
}