#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include "Account.h"
#include "md5.h"

#define MAX_command 20
#define MAX_LENGTH 120

const static char table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
							 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
							 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
							 'U', 'V', 'W', 'X', 'Y', 'Z',
							 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
							 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
							 'u', 'v', 'w', 'x', 'y', 'z'};
							 
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

using namespace std;

typedef unordered_map <string, Account> Bankunordered_map;
typedef unordered_map <string, Account>::iterator Bankiterator;
Bankunordered_map banksystem;

void recommend_create(const string& id)
{

	std::string test(id);
	std::vector <std::string> recommend;
	int firstlastcharindex = 0;
	int secondlastcharindex = 0;
	int thirdlastcharindex = 0;
	
	if(id.length() > 0){
		while(table[firstlastcharindex] != id[id.length() - 1])
			firstlastcharindex++;
	}
	if(id.length() > 1){
		while(table[secondlastcharindex] != id[id.length() - 2])
			secondlastcharindex++;
	}
	if(id.length() > 2){
		while(table[thirdlastcharindex] != id[id.length() - 3])
			thirdlastcharindex++;
	}

	//score 1 : delete last char
	if(test.length() > 1){
		test.erase(test.end() - 1);

		if(banksystem.find(test) == banksystem.end() && test.size() != 0)
			recommend.push_back(test);
	}

	//score 1 : change last char with a char prior to the original one
	test = id;
	test.erase(test.end() - 1);

	for(int i = 0; recommend.size() <= 10 && table[i] != id[id.length() - 1] && i < sizeof(table)/sizeof(char); i++){
		test.push_back(table[i]);

		if(banksystem.find(test) == banksystem.end())
			recommend.push_back(test);

		test.erase(test.end() - 1);
	}

	if(recommend.size() >= 10){
		for(int i = 0; i < 9; i++){
			std::cout << recommend[i] << ',';
		}
		std::cout << recommend[9] << std::endl;
		return;
	}
	//score 1 : add one char
	test = id;

	for(int i = 0; recommend.size() <= 10 && i < sizeof(table)/sizeof(char); i++){
		test.push_back(table[i]);

		if(banksystem.find(test) == banksystem.end())
			recommend.push_back(test);

		test = id;
	}	

	if(recommend.size() >= 10){
		for(int i = 0; i < 9; i++){
			std::cout << recommend[i] << ',';
		}
		std::cout << recommend[9] << std::endl;
		return;
	}
	//score 1 : change last char with a char with lower priority with original one 
	test = id;
	test.erase(test.end() - 1);
	
	for(int i = firstlastcharindex + 1; recommend.size() <= 10 && i < sizeof(table)/sizeof(char); i++){
		test.push_back(table[i]);

		if(banksystem.find(test) == banksystem.end())
			recommend.push_back(test);

		test.erase(test.end() - 1);	
	}

	if(recommend.size() >= 10){
		for(int i = 0; i < 9; i++){
			std::cout << recommend[i] << ',';
		}
		std::cout << recommend[9] << std::endl;
		return;
	}

	//score 2: delete last char and change the last char of the new string
	//score 2: change the second-last char
	test = id;
	if(test.length() >= 2){

		for(int i = 0; recommend.size() <= 10 && i < secondlastcharindex; i++){
			//remove the last char and replace the new string's last char
			test.erase(test.end() - 1);
			
			test[test.length() - 1] = table[i];

			if(banksystem.find(test) == banksystem.end())
				recommend.push_back(test);

			test.push_back(id[id.length() - 1]);

			if(banksystem.find(test) == banksystem.end())
				recommend.push_back(test);
		}

		if(recommend.size() >= 10){
			for(int i = 0; i < 9; i++){
				std::cout << recommend[i] << ',';
			}
			std::cout << recommend[9] << std::endl;
			return;
		}
	}

	//score 2: change the last char of the string and add a char
	test = id;
	for(int i = 0; recommend.size() <= 10 && i < firstlastcharindex; i++){
		test[test.length() - 1] = table[i];
		for(int j = 0; recommend.size() <= 10 && j < sizeof(table)/sizeof(char); j++){
			test.push_back(table[j]);
			if(banksystem.find(test) == banksystem.end())
				recommend.push_back(test);
			test.erase(test.end() - 1);
		}
	}

	if(recommend.size() >= 10){
		for(int i = 0; i < 9; i++){
			std::cout << recommend[i] << ',';
		}
		std::cout << recommend[9] << std::endl;
		return;
	}
	//score 2: change the last char of the string and add a char(priority after original)
	test = id;
	for(int i = firstlastcharindex + 1; recommend.size() <= 10 && i < sizeof(table)/sizeof(char); i++){
		test[test.length() - 1] = table[i];
		for(int j = 0; recommend.size() <= 10 && j < sizeof(table)/sizeof(char); j++){
			test.push_back(table[j]);
			if(banksystem.find(test) == banksystem.end())
				recommend.push_back(test);
			test.erase(test.end() - 1);
		}
	}

	if(recommend.size() >= 10){
		for(int i = 0; i < 9; i++){
			std::cout << recommend[i] << ',';
		}
		std::cout << recommend[9] << std::endl;
		return;
	}

	//score 2: delete last char and change the last char of the new string(prioirty after original)
	//score 2: change the second-last char(prioirty after original)
	test = id;
	if(test.length() >= 2){

		for(int i = secondlastcharindex + 1; recommend.size() <= 10 && i < sizeof(table)/sizeof(char); i++){
			//remove the last char and replace the new string's last char
			test.erase(test.end() - 1);
			
			test[test.length() - 1] = table[i];

			if(banksystem.find(test) == banksystem.end())
				recommend.push_back(test);

			test.push_back(id[id.length() - 1]);

			if(banksystem.find(test) == banksystem.end())
				recommend.push_back(test);
		}

		if(recommend.size() >= 10){
			for(int i = 0; i < 9; i++){
				std::cout << recommend[i] << ',';
			}
			std::cout << recommend[9] << std::endl;
			return;
		}
	}

	//score 3: delete the last char and change the second-last char of new string 
	//score 3: change the third-last char
	
	test = id;
	
	if(test.length() >= 3){

		test.erase(test.end() - 1);
		

		for(int i = 0; recommend.size() <= 10 && i < thirdlastcharindex; i++){
			
			//remove the last char and replace the new string's last char
			test[test.length() - 2] = table[i];

			if(banksystem.find(test) == banksystem.end())
				recommend.push_back(test);

			test.push_back(id[id.length() - 1]);

			if(banksystem.find(test) == banksystem.end())
				recommend.push_back(test);

			test.erase(test.end() - 1);
		
			
		}

		if(recommend.size() >= 10){
			for(int i = 0; i < 9; i++){
				std::cout << recommend[i] << ',';
			}
			std::cout << recommend[9] << std::endl;
			return;
		}
	}
	//score 3: delete last two char
	test = id;
	if(test.length() > 2){
		test.erase(test.end() - 1);
		test.erase(test.end() - 1);

		if(banksystem.find(test) == banksystem.end())
			recommend.push_back(test);

		if(recommend.size() >= 10){
			for(int i = 0; i < 9; i++){
				std::cout << recommend[i] << ',';
			}
			std::cout << recommend[9] << std::endl;
			return;
		}
	}
	//score 3: change the last two char
	test = id;
	if(test.length() > 1){
		for(int i = 0; recommend.size() <= 10 && i < secondlastcharindex; i++){
			test[test.length() - 2] = table[i];
			
			for(int j = 0; recommend.size() <= 10 && j < sizeof(table)/sizeof(char); j++){
				if(j != firstlastcharindex){
					test[test.length() - 1] = table[j];
					
					if(banksystem.find(test) == banksystem.end())
						recommend.push_back(test);
				}
			}
		}

		if(recommend.size() >= 10){
			for(int i = 0; i < 9; i++){
				std::cout << recommend[i] << ',';
			}
			std::cout << recommend[9] << std::endl;
			return;
		}
	}
	//score 3: add two char
	test = id;
	for(int i = 0; recommend.size() <= 10 && i < sizeof(table)/sizeof(char); i++){
		test.push_back(table[i]);
		for(int j = 0; recommend.size() <= 10 && j < sizeof(table)/sizeof(char); j++){
			test.push_back(table[j]);
			if(banksystem.find(test) == banksystem.end())
					recommend.push_back(test);

			test.erase(test.end() - 1);
		}

		test.erase(test.end() - 1);
	}

	if(recommend.size() >= 10){
		for(int i = 0; i < 9; i++){
			std::cout << recommend[i] << ',';
		}
		std::cout << recommend[9] << std::endl;
		return;
	}
	//score 3: change last two char(priority after original)
	test = id;
	if(test.length() > 1){
		for(int i = secondlastcharindex + 1; recommend.size() <= 10 && i < sizeof(table)/sizeof(char); i++){
			test[test.length() - 2] = table[i];
			
			for(int j = 0; recommend.size() <= 10 && j < sizeof(table)/sizeof(char); j++){
				if(j != firstlastcharindex){
					test[test.length() - 1] = table[j];
					
					if(banksystem.find(test) == banksystem.end())
						recommend.push_back(test);
				}
			}
		}

		if(recommend.size() >= 10){
			for(int i = 0; i < 9; i++){
				std::cout << recommend[i] << ',';
			}
			std::cout << recommend[9] << std::endl;
			return;
		}
	}
	//score 3: change the third-last char and delete the last char(priority after original)
	//score 3: change the third-last char(priority after original)
	test = id;
	
	if(test.length() > 2){
		for(int i = thirdlastcharindex + 1; recommend.size() <= 10 && i < sizeof(table)/sizeof(char); i++){

			test[test.length() - 3] = table[i];
			test.erase(test.end() - 1);
		
			if(banksystem.find(test) == banksystem.end())
				recommend.push_back(test);

			test.push_back(id[id.length() - 1]);

			if(banksystem.find(test) == banksystem.end())
				recommend.push_back(test);
			
		}


		if(recommend.size() >= 10){
			for(int i = 0; i < 9; i++){
				std::cout << recommend[i] << ',';
			}
			std::cout << recommend[9] << std::endl;
			return;
		}
	}
}

void isMatch(Bankiterator it, const Bankiterator end, const std::string& wildcard, const std::string& last)
{
	vector <std::string> wild;

	for(; it != end; it++)
		if(wildcmp(wildcard, it->first) && it->first != last)
			wild.push_back(it->first);

	sort(wild.begin(), wild.end(), 
		[](const std::string& a, const std::string& b){
			int key = a.compare(b);
			if(k < 0) return true;
			return false;
		});

	int size = wild.size();

	if(size != 0)
		cout << wild[0];

	for(int i = 1; i < size; i++)
		cout << "," << wild[i];

	printf("\n");
}

inline int score(const string& a, const string& b)
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

inline void swap(pair <string, int>& a, pair <string, int>& b)
{
	pair <string, int> temp = a;
	a = b;
	b = temp;
}

void recommend_transfer(Bankiterator it, const Bankiterator end, int mapsize, const string& ID2)
{
	vector<pair<string, int> >recommend;
	recommend.reserve(mapsize);

	for(; it != end; it++)
		recommend.push_back(make_pair(it->first, score(it->first, ID2)));
	// apply selection sort to find the top 10
	int min;
	for(int i = 0; i < 10 && i < mapsize-1; i++){
		min = i;
		for(int j = i+1; j < mapsize; j++)
			if(recommend[min].second > recommend[j].second || 
			  (recommend[min].second == recommend[j].second && recommend[min].first.compare(recommend[j].first) > 0))
				min = j;
		if(min != i)
			swap(recommend[min], recommend[i]);
	}
	if(mapsize != 0)
		cout << recommend[0].first;
	for(int i = 1; i < 10 && i < mapsize; i++)
		cout << "," << recommend[i].first;
	printf("\n");
}

int main()
{
	char command[MAX_command];
	string id, password, _id, _password;
	string lastID;
	string wildcard;
	int money;
	long long int transfer_time = (long long int)(0);
	Bankiterator nowAccount, toAccount, currentAccount;

	while(scanf("%s", command) != EOF){
		
		if(strcmp(command, "login") == 0){
			cin >> id >> password;
			nowAccount = banksystem.find(id);

			if(nowAccount != banksystem.end()){
				if(nowAccount->second.login(md5(password))){
					currentAccount = banksystem.find(id);
					lastID = id;
				}
			}
			else cout << "ID " << id << " not found" << endl;
		}
		else if(strcmp(command, "create") == 0){
			cin >> id >> password;
			nowAccount = banksystem.find(id);

			if(nowAccount != banksystem.end()){
				cout << "ID " << id << " exists, ";
				recommend_create(id);
			}
			else{
				Account newAccount(md5(password));
				banksystem.insert(make_pair(id, newAccount));
				printf("success\n");
			}
		}
		else if(strcmp(command, "delete") == 0){
			cin >> id >> password;
			nowAccount = banksystem.find(id);

			if(nowAccount != banksystem.end()){
				if(nowAccount->second.checkpassword(md5(password))){
					banksystem.erase(nowAccount);
					printf("success\n");
				}
				else printf("wrong password\n");
			}
			else cout << "ID " << id << " not found" << endl;
		}
		else if(strcmp(command, "merge") == 0){
			cin >> id >> password >> _id >> _password;

			nowAccount = banksystem.find(id);
			toAccount = banksystem.find(_id);

			if(nowAccount != banksystem.end() && toAccount != banksystem.end()){
				money = nowAccount->second.merge(toAccount->second, md5(password), md5(_password));
				
				if(money >= 0){
					Bankiterator it;
					cout << "success, " << id << " has " << money << " dollars" << endl;

					vector <history> record = toAccount->second.All_record;
					int size = record.size();

					for(int i = 0; i < size; i++){
						it = banksystem.find(record[i].ID);
						if(it != banksystem.end())
							it->second.changeID(id, record[i].time);
					}

					banksystem.erase(toAccount);
				}
			}
			else if(nowAccount == banksystem.end())
				cout << "ID " << id << " not found" << endl;
			else
				cout << "ID " << _id << " not found" << endl;
 		}
		else if(strcmp(command, "deposit") == 0){
			scanf("%d", &money);
			currentAccount->second.deposit(money);
		}
		else if(strcmp(command, "withdraw") == 0){
			scanf("%d", &money);
			currentAccount->second.withdraw(money);
		}
		else if(strcmp(command, "transfer") == 0){
			cin >> id >> money;
			toAccount = banksystem.find(id);

			if(toAccount != banksystem.end()){
				currentAccount->second.transfer(toAccount->second, lastID, id, money, transfer_time);
				transfer_time++;
			}
			else{
				cout << "ID " << id << " not found, ";
				Bankiterator it = banksystem.begin(), end = banksystem.end();
				int mapsize = banksystem.size();
				recommend_transfer(it, end, mapsize, id);
			}
		}
		else if(strcmp(command, "find") == 0){
			cin >> wildcard;
			Bankiterator it = banksystem.begin(), end = banksystem.end();
			isMatch(it, end, wildcard, lastID);
		}
		else if(strcmp(command, "search") == 0){
			cin >> id;
			currentAccount->second.search(id);
		}
	}

	return 0;
}