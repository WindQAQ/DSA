#include <string>
#include <vector>
#include <cstdio>
#include <utility>
#include <iostream>

typedef struct history{
	int type; //1 is from, 2 is to
	std::string ID;
	int money_transfer;
	long long int time;
}history;

class Account{

public:
	std::vector <history> All_record;
	Account(const std::string&);
	void deposit(int);
	void withdraw(int);
	void transfer(Account&, const std::string&, const std::string&, int, long long int);
	void search(const std::string&);
	bool login(const std::string&);
	int merge(Account&, const std::string&, const std::string&);
	void changeID(const std::string&, long long int);
	bool checkpassword(const std::string&);
private:
	std::string password; // after MD5 hash
	int money;
	inline void insert_and_sort(std::vector<history>&);
};