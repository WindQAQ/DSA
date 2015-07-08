#include <utility>
#include "Account.h"

Account::Account(const std::string& _password)
{
	password = _password;
	money = 0;
	All_record.reserve(10);
}


void Account::deposit(int amount)
{
	money += amount;
	printf("success, %d dollars in current account\n", money);
	return;
}

void Account::withdraw(int amount)
{
	if(money < amount){
		printf("fail, %d dollars only in current account\n", money);
	}
	else{
		money -= amount;
		printf("success, %d dollars left in current account\n", money);	
	}

	return;
}

void Account::transfer(Account& toAccount, const std::string& fromID, const std::string& toID, int num, long long int transfertime)
{
	if(money < num){
		printf("fail, %d dollars only in current account\n", money);
	}
	else{
		// FROM
		money -= num;
		
		history newrecord;
		newrecord.type = 2;
		newrecord.ID = toID;
		newrecord.money_transfer = num;
		newrecord.time = transfertime;

		All_record.push_back(newrecord);

		// TO
		toAccount.money += num;

		newrecord.type = 1;
		newrecord.ID = fromID;
		newrecord.money_transfer = num;
		newrecord.time = transfertime;

		toAccount.All_record.push_back(newrecord);

		printf("success, %d dollars left in current account\n", money);
	}

	return;
}

void Account::search(const std::string& inputID)
{
	std::string printID;
	int printmoney;
	bool found = false;
	int size = All_record.size();
	for(int i = 0; i < size; i++){
		if(All_record[i].ID == inputID){
			found = true;
			if(All_record[i].type == 1){
				printID = All_record[i].ID;
				printmoney = All_record[i].money_transfer;
				printf("From %s %d\n", printID.c_str(), printmoney);
			}
			else{
				printID = All_record[i].ID;
				printmoney = All_record[i].money_transfer;
				printf("To %s %d\n", printID.c_str(), printmoney);
			}	
		}
	}

	if(!found)
		printf("no record\n");

	return;
}

bool Account::login(const std::string& md5password)
{
	if(checkpassword(md5password)){
		printf("success\n");
		return true;
	}
	else{
		printf("wrong password\n");
		return false;
	}
}

bool Account::checkpassword(const std::string& input)
{
	if(input == password)
		return true;
	else
		return false;
}

void Account::insert_and_sort(std::vector<history> &ID2)
{
	std::vector<history> result;
	result.reserve(All_record.size()+ID2.size());
	std::vector<history>::iterator it1;
	std::vector<history>::iterator it2;
	it1 = this->All_record.begin();
	it2 = ID2.begin();

	while(it1 != this->All_record.end() && it2 != ID2.end()){
		if(it1->time < it2->time){
			result.push_back(*it1);
			it1++;
		}
		else if(it1->time > it2->time){
			result.push_back(*it2);
			it2++;
		}
		else if (it1->time == it2->time){
			result.push_back(*it1);
			result.push_back(*it2);
			it1++;
			it2++;
		}
	}
	
	while(it1 != this->All_record.end()){
		result.push_back(*it1);
		it1++;
	}
	
	while(it2 != ID2.end()){
		result.push_back(*it2);
		it2++;	
	}

	this->All_record = result;

	return;
}

int Account::merge(Account &ID2, const std::string &password1, const std::string &password2)
{
	if(!checkpassword(password1)){
		printf("wrong password1\n");
		return -1;
	}

	if(!ID2.checkpassword(password2)){
		printf("wrong password2\n");
		return -1;
	}

	money += ID2.money;

	insert_and_sort(ID2.All_record);

	return money;
}

void Account::changeID(const std::string& ID1, long long int time)
{
	int size = All_record.size();
	
	if(size == 0)
		return;

	int left = 0;
	int right = All_record.size() - 1;
	int middle = (left + right) / 2;

	if(All_record[0].time == time){
		All_record[0].ID = ID1;
		if(1 < size && All_record[1].time == time)
			All_record[1].ID = ID1;
		return;
	}

	if(All_record[right].time == time){
		All_record[right].ID = ID1;
		if(right-1 >= 0 && All_record[right-1].time == time)
			All_record[right-1].ID = ID1;
		return;
	}

	while(All_record[middle].time != time && left < right){
		if(All_record[middle].time > time)
			right = middle - 1;
		else if(All_record[middle].time < time)
			left = middle + 1;
		middle = (left + right) / 2;
	}
	
	if(All_record[middle].time == time){
		All_record[middle].ID = ID1;
		if(middle-1 >= 0 && All_record[middle-1].time == time)
			All_record[middle-1].ID = ID1;
		if(middle+1 < size && All_record[middle+1].time == time)
			All_record[middle+1].ID = ID1;
	}

	return;
}