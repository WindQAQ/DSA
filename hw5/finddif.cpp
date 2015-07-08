#include <iostream>
#include <string>
#include <cassert>

using namespace std;

bool complastchar(string str1, string str2, int k)
{
	int end = str1.size() - 1;
	for(int i = end; i >= end-k+1; i--)
		if(str1[i] != str2[i])
			return false;
	return true;
}

void assignate(string& str, int start, int end)
{
	string temp(str);
	str.clear();
	for(int i = start; i <= end; i++)
		str.push_back(temp[i]);
}

int FindDif(string str1, string str2, int start)
{
	assert(str1.size() == str2.size());
	int length = str1.size();
	//cout << "str1: " << str1 << ", " << "str2: " << str2 << endl;
	//cout << "start = " << start << ", " << "length = " << length << endl;
	if(complastchar(str1, str2, length-1))
		return start;
	if(!complastchar(str1, str2, 1)){
		return (start + length - 1);
	}
	if(complastchar(str1, str2, length/2)){
		str1.assign(str1, 0, length/2+1);
		str2.assign(str2, 0, length/2+1);
		return FindDif(str1, str2, start);
	}
	else{
		str1.assign(str1, length/2, length - length/2);
		str2.assign(str2, length/2, length - length/2);
		return FindDif(str1, str2, start + length/2);
	}
}

int FindDif1(string str1, string str2, int start)
{
	assert(str1.size() == str2.size());
	int length = str1.size();
	//cout << "str1: " << str1 << ", " << "str2: " << str2 << endl;
	//cout << "start = " << start << ", " << "length = " << length << endl;
	if(complastchar(str1, str2, length-1))
		return start;
	if(!complastchar(str1, str2, 1))
		return (start + length - 1);
	if(complastchar(str1, str2, length/2)){
		assignate(str1, 0, length/2);
		assignate(str2, 0, length/2);
		return FindDif1(str1, str2, start);
	}
	else{
		assignate(str1, length/2, length - 1);
		assignate(str2, length/2, length - 1);
		return FindDif1(str1, str2, start + length/2);
	}
}

int main()
{

	string str1, str2;
	while(getline(cin, str1)){
		getline(cin, str2);
		cout << "==========\n";
		//cout << "str1: " << str1 << endl << "str2: " << str2 << endl;
		cout << "different position: " << FindDif1(str1, str2, 0) << endl;
		cout << "==========\n";
	}
}