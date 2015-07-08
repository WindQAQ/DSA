#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>

using namespace std;

typedef pair <string, int> mypair;

bool comp(const mypair& L, const mypair& R)
{
	if(L.second < R.second)
		return true;
	else if(L.second == R.second)
		assert(1);
	return false;
}

int HashFunc(string str){
    int sum = 0;
   	for(int i = 0; i < str.size(); i++) sum += str[i] - 'd';
    sum %= 32;
    if(sum >= 10 && sum <= 14) sum++;
    if(sum >= 12 && sum <= 17) sum++;
    if(sum == 0 && str[0] == 'l') sum = 23;
    if(sum == 5) sum = (sum + str[0] - 'b') % 11;
    if(sum == 8 || sum == 9) sum = sum + (str[0] - 'a') % 4;
    if(sum == 2 && str[0] == 'v') sum = 1;
    if(sum == 2 && str[0] == 'f') sum = 22;
    if(sum == 21 && str[0] == 't') sum = 20;
    if(sum == 9 && str[0] == 'a') sum = 5;
	return sum;    
}

int main()
{
	string str;
	vector <mypair> out;

	while(getline(cin, str))
		out.push_back(make_pair(str, HashFunc(str)));

	sort(out.begin(), out.end(), comp);

	for(vector <mypair>::iterator it = out.begin(); it != out.end(); it++)
		cout << it->first << ": " << it->second << endl;
	return 0;
}