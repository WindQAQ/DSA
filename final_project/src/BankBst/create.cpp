#include <iostream>
#include <string>
#include "Account.h"
extern "C"
{
#include "bst.h"
}

const static char table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
							 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
							 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
							 'U', 'V', 'W', 'X', 'Y', 'Z',
							 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
							 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
							 'u', 'v', 'w', 'x', 'y', 'z'};

void recommend_create(struct bst_table* banksystem, const std::string& id)
{

	std::string test(id);
	std::vector <std::string> recommend;
	int firstlastcharindex = 0;
	int secondlastcharindex = 0;
	int thirdlastcharindex = 0;
	Account t;
	
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

		t.id = test; if(bst_find(banksystem, &t) == NULL && test.size() != 0)
			recommend.push_back(test);
	}

	//score 1 : change last char with a char prior to the original one
	test = id;
	test.erase(test.end() - 1);

	for(int i = 0; recommend.size() <= 10 && table[i] != id[id.length() - 1] && i < sizeof(table)/sizeof(char); i++){
		test.push_back(table[i]);

		t.id = test; if(bst_find(banksystem, &t) == NULL)
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

		t.id = test; if(bst_find(banksystem, &t) == NULL)
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

		t.id = test; if(bst_find(banksystem, &t) == NULL)
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

			t.id = test; if(bst_find(banksystem, &t) == NULL)
				recommend.push_back(test);

			test.push_back(id[id.length() - 1]);

			t.id = test; if(bst_find(banksystem, &t) == NULL)
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
			t.id = test; if(bst_find(banksystem, &t) == NULL)
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
			t.id = test; if(bst_find(banksystem, &t) == NULL)
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

			t.id = test; if(bst_find(banksystem, &t) == NULL)
				recommend.push_back(test);

			test.push_back(id[id.length() - 1]);

			t.id = test; if(bst_find(banksystem, &t) == NULL)
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

			t.id = test; if(bst_find(banksystem, &t) == NULL)
				recommend.push_back(test);

			test.push_back(id[id.length() - 1]);

			t.id = test; if(bst_find(banksystem, &t) == NULL)
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

		t.id = test; if(bst_find(banksystem, &t) == NULL)
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
					
					t.id = test; if(bst_find(banksystem, &t) == NULL)
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
			t.id = test; if(bst_find(banksystem, &t) == NULL)
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
					
					t.id = test; if(bst_find(banksystem, &t) == NULL)
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
		
			t.id = test; if(bst_find(banksystem, &t) == NULL)
				recommend.push_back(test);

			test.push_back(id[id.length() - 1]);

			t.id = test; if(bst_find(banksystem, &t) == NULL)
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