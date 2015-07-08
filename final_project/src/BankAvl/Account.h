#include <string>
#include <vector>

class Account{
public:
	std::string id;
	bool check_psd(std::string);
	void merge_hist(Account*);
	void deposit(int);
	void withdraw(int);
	void search(const std::string&);
	void transfer(Account*, int, int);
	void hist_null();
	int compare(const Account&);
	Account& operator=(const Account&);
	Account(){};
	Account(const std::string& _id, const std::string& _psd)
	: id(_id), psd(_psd), balance(0){};
private:
	typedef struct log{
		char type;
		std::string id;
		int num;
		int prior;
		struct log *link;
		log(){};
		log(char _t, std::string& _i, int _n, int _p): type(_t), id(_i), num(_n), prior(_p), link(NULL){};
	} Log;
	std::string psd;
	int balance;
protected:
	std::vector <Log*> history;
};