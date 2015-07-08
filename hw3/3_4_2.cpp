#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <deque>
#include <stack>

using namespace std;

bool judgeunary(char c)
{
	char op[] = {'+', '-', '*', '/', '%', '(', ','};

	for(int i = 0; i < 7; i++)
		if(c == op[i])
			return true;
	return false;
}

int priority(const string& input)
{
	if(input.size() >= 3) return 2;
	if(input == "p") return 3;
	if(input == "n") return 3;
	if(input == "*") return 5;
	if(input == "/") return 5;
	if(input == "%") return 5;
	if(input == "+") return 6;
	if(input == "-") return 6;
	if(input == "(") return 15;

	return 0;
}

void print_postfix(deque<string> o1)
{
	int size = o1.size();
	cout << "Postfix Exp:";
	while(size > 0){
		if(o1.front() == "p")
			cout << " +";
		else if(o1.front() == "n")
			cout << " -";
		else if(isdigit(o1.front().at(0)))
			printf(" %.6lf", atof(o1.front().c_str()));
		else
			cout << " " << o1.front();
		o1.push_back(o1.front());
		o1.pop_front();
		size--;
	}
	cout << endl;
}

void print_o1(deque<string> o1)
{
	int size = o1.size();
	cout << "\tcurrent output:";
	while(size > 0){
		if(o1.front() == "p")
			cout << " +,";
		else if(o1.front() == "n")
			cout << " -,";
		else if(isdigit(o1.front().at(0)))
			printf(" %.6lf,", atof(o1.front().c_str()));
		else
			cout << " " << o1.front() << ",";
		o1.push_back(o1.front());
		o1.pop_front();
		size--;
	}
	cout << endl;
}

void print_o2(stack<string>& s, stack<string>& o2)
{
	cout << "\tcurrent stack:";
	while(!s.empty()){
		o2.push(s.top());
		s.pop();
	}
	while(!o2.empty()){
		if(o2.top() == "p")
			cout << " +,";
		if(o2.top() == "n")
			cout << " -,";
		else 
			cout << " " << o2.top() << ",";
		s.push(o2.top());
		o2.pop();
	}
	cout << endl;
}

void check(stack<string>& s, const string& input, deque<string>& o1, stack<string>& o2)
{
	if(input == "p")
		cout << "encounter +: push to stack" << endl;
	else if(input == "n")
		cout << "encounter -: push to stack" << endl;
	else
		cout << "encounter " << input << ": push to stack" << endl;
	if(!s.empty() && !(priority(s.top()) == 3 && priority(input) == 3))
 		if(!s.empty() && (priority(s.top()) <= priority(input))){
 			cout << "\t*** stack.top() has greater precedence ***" << endl;
			while(!s.empty() && (priority(s.top()) <= priority(input))){
				o1.push_back(s.top());
				s.pop();
			}
		}
	s.push(input);
	print_o1(o1);
	print_o2(s, o2);
}

int is_single(const string& s)
{
	string op[] = {"p", "n", "sin", "cos", "exp", "log", "sqrt", "fabs"};
	for(int i = 0; i < 8; i++)
		if(s == op[i])
			return (i+1);
	return 0;
}

void do_single(stack<double>& s, deque<string>& o1)
{
	double t;
	int k = is_single(o1.front());
	if(k == 1) t = s.top();
	if(k == 2) t = -s.top();
	if(k == 3) t = sin(s.top());
	if(k == 4) t = cos(s.top());
	if(k == 5) t = exp(s.top());
	if(k == 6) t = log(s.top());
	if(k == 7) t = sqrt(s.top());
	if(k == 8) t = fabs(s.top());
	s.pop();
	s.push(t);
}

int is_binary(const string& s)
{
	const string op[] = {"+", "-", "*", "/", "pow"};

	for(int i = 0; i < 5; i++)
		if(s == op[i])
			return (i+1);
	return 0;
}

void do_binary(stack<double>& s, deque<string>& o1)
{
	double t, a, b;
	int k = is_binary(o1.front());
	b = s.top();
	s.pop();
	a = s.top();
	s.pop();
	if(k == 1) t = a + b;
	if(k == 2) t = a - b;
	if(k == 3) t = a * b;
	if(k == 4) t = a / b;
	if(k == 5) t = pow(a, b); 
	s.push(t);
}

void eval_result(deque<string>& o1)
{
	stack<double> s;
	while(!o1.empty()){
		if(!is_binary(o1.front()) && !is_single(o1.front()))
			s.push(atof(o1.front().c_str()));
		else if(is_single(o1.front()))
			do_single(s, o1);
		else if(is_binary(o1.front()))
			do_binary(s, o1);
		o1.pop_front();
	}
	printf("RESULT: %.6lf\n", s.top());
}

int main()
{
	string origin, input;
	stack<string> s, r, o2;
	deque<string> o1;
	
	while(getline(cin, origin)){
		cout << "# transform from infix to postfix" << endl;

		for(int i = 0; i < origin.size(); i++)
			if(origin[i] != ' ')
				input.push_back(origin[i]);
		for(int i = 0; i < input.size(); i++){
			if(input[i] != ' '){
				if(input[i] == '+' || input[i] == '-'){
					if(i == 0 && input[i] == '+')
						check(s, "p", o1, o2);
					else if(i == 0 && input[i] == '-')
						check(s, "n", o1, o2);
					else if(i != 0 && input[i] == '+' && judgeunary(input[i-1]))
						check(s, "p", o1, o2);
					else if(i != 0 && input[i] == '-' && judgeunary(input[i-1]))
						check(s, "n", o1, o2);
					else{
						string t(1, input[i]);
						check(s, t, o1, o2);
					}
				}
				else if(isdigit(input[i]) || input[i] == '.'){
					string t;
					for(int j = 0; isdigit(input[i]) || input[i] == '.'; i++, j++)
						t += input[i];
					i--;
					o1.push_back(t);
					cout << "encounter " << t << ": push to output" << endl;
					print_o1(o1);
					print_o2(s, o2);
				}
				else if(input[i] == '('){
					cout << "encounter (: push to stack" << endl;
					s.push("(");
					print_o1(o1);		
					print_o2(s, o2);
				}
				else if(input[i] == ')'){
					cout << "encounter ): flush the stack to output until meeting '('" << endl;
					while(!s.empty() && s.top() != "("){
						o1.push_back(s.top());
						s.pop();
					}
					if(!s.empty())
						s.pop();
					if(!s.empty()){
						if(priority(s.top()) == 2){
							o1.push_back(s.top());
							s.pop();
						}
					}
					print_o1(o1);
					print_o2(s, o2);
				}
				else if(isalpha(input[i])){
					string t;
					if(input[i] != 's'){
						t += input[i];
						t += input[i+1];
						t += input[i+2];
						i += 2;
						if(t[0] == 'f'){
							t += 's';
							i++;
						}
					}
					else if(input[i] == 's'){
						t += input[i];
						t += input[i+1];
						t += input[i+2];
						if(t == "sin")
							i += 2;	
						else{
							t = "sqrt";
							i += 3;
						}							
					}
					check(s, t, o1, o2);
				}
				else if(input[i] == ','){
					cout << "encounter ,: flush the stack to output until meeting '('" << endl;
					while(!s.empty() && s.top() != "("){
						o1.push_back(s.top());
						s.pop();
					}
				}
				else{
					string t(1, input[i]);
					check(s, t, o1, o2);
				}
			}
		}
		cout << "encounter NOTHING: flush the stack to output" << endl;
		while(!s.empty()){
			o1.push_back(s.top());
			s.pop();
		}
		print_o1(o1);
		cout << "# postfix expression transforming complete" << endl;
		print_postfix(o1);
		eval_result(o1);
		origin.clear();
		input.clear();
		o1.clear();
	}
}	