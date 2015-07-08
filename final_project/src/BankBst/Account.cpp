#include <iostream>
#include "Account.h"

bool Account::check_psd(std::string _psd)
{
	return (psd == _psd);
}

void Account::hist_null()
{
	int size = history.size();

	for(int i = 0; i < size; i++)
		history[i] = NULL;
}

void Account::merge_hist(Account* acc_in)
{
	int size1 = history.size(), size2 = acc_in->history.size();
	int i = 0, j = 0;
	std::vector <Log*> result;
	result.reserve(size1+size2);

	while(i < size1 && j < size2){
		if(history[i]->prior < acc_in->history[j]->prior){
			result.push_back(history[i]);
			i++;
		}
		else if(history[i]->prior > acc_in->history[j]->prior){
			if(acc_in->history[j]->link != NULL)
				acc_in->history[j]->link->id = id;
			result.push_back(acc_in->history[j]);
			j++;
		}
		else if(history[i]->prior == acc_in->history[j]->prior){
			if(acc_in->history[j]->link != NULL)
				acc_in->history[j]->link->id = id;
			result.push_back(history[i]);
			result.push_back(acc_in->history[j]);
			i++;
			j++;
		}
	}

	while(i != size1){
		result.push_back(history[i]);
		i++;
	}

	while(j != size2){
		if(acc_in->history[j]->link != NULL)
			acc_in->history[j]->link->id = id;
		result.push_back(acc_in->history[j]);
		j++;
	}

	balance += acc_in->balance;
	history = result;
	std::cout << "success, " << id << " has " << balance << " dollars" << std::endl;
}
void Account::deposit(int num)
{
	balance += num;
	std::cout << "success, " << balance << " dollars in current account" << std::endl;
}

void Account::withdraw(int num)
{
	if(balance < num){
		std::cout << "fail, " << balance << " dollars only in current account" << std::endl;
		return;
	}
	balance -= num;
	std::cout << "success, " << balance << " dollars left in current account" << std::endl;
}

void Account::transfer(Account* acc_in, int num, int time)
{
	if(balance < num){
		std::cout << "fail, " << balance << " dollars only in current account" << std::endl;
		return;
	}
	balance -= num;
	acc_in->balance += num;
	Log *T = new Log('T', acc_in->id, num, time);
	Log *F = new Log('F', id, num, time);
	T->link = F;
	F->link = T;
	history.push_back(T);
	acc_in->history.push_back(F);
	std::cout << "success, " << balance << " dollars left in current account" << std::endl;
}

int Account::compare(const Account& acc_in)
{
	return id.compare(acc_in.id);
}

void Account::search(const std::string& str)
{
	int size = history.size();
	bool found = false;

	for(int i = 0; i < size; i++){
		if(history[i]->id == str){
			found = true;
			if(history[i]->type == 'T')
				std::cout << "To " << str << " " << history[i]->num << std::endl;
			else
				std::cout << "From " << str << " " << history[i]->num << std::endl;
		}
	}

	if(!found)
		std::cout << "no record" << std::endl;
}

Account& Account::operator=(const Account& acc_in)
{
	id = acc_in.id;
	psd = acc_in.psd;
	balance = acc_in.balance;
	history = acc_in.history;
}