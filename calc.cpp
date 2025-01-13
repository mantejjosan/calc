#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace std;

bool isOperator(char& c){
	switch(c){
		case '+': case '-': case '*': case '/': case '^': return true;
	}
	return false;
}

bool isDigit(char& c){
	if((c>=48 && c<=57) || c == '.')
		return true;
	return false;
}

vector<string> tokenize(string exp){
	vector<string> vec;
	string num = "";

	for(int i=0; i<exp.length(); i++){
		if(isOperator(exp.at(i))){
			vec.push_back(num);
			num = "";
			vec.push_back(string(1, exp.at(i)));
		}
		else if(!isDigit(exp.at(i))){
			cout << "Invalid character in expression";
			return {};
		}
		else
		num += exp.at(i);
	}
	if(!num.empty())
		vec.push_back(num);
	return vec;
}

double calc(double _1, double _2, char op){
	switch(op){
		case '+': return _1 + _2; 
		case '-': return _1 - _2;
		case '*': return _1 * _2;
		case '/': 
			  if(_2 == 0){
				  cout << "Undefined";
				  return numeric_limits<double>::quiet_NaN();
			  }
			  return _1 / _2;
		case '^': 
			  return pow(_1, _2);
		default: return numeric_limits<double>::quiet_NaN();
	}
}
void logic(vector<string>& exp) {
    bool isDone = false;

    // First pass: Handle multiplication (*) and division (/)
    while (!isDone) {
        isDone = true;  // Assume we are done unless an operation is performed
        for (size_t i = 0; i < exp.size(); i++) {
            string& x = exp.at(i);
            if (x == "/" || x == "*") {
                // Perform the operation
                double num = calc(stod(exp.at(i - 1)), stod(exp.at(i + 1)), x.at(0));
                exp[i - 1] = to_string(num);           // Replace left operand with result
                exp.erase(exp.begin() + i, exp.begin() + i + 2);  // Erase operator and right operand
                isDone = false;  // Another pass is needed
                break;
            }
        }
    }

    // Second pass: Handle addition (+) and subtraction (-)
    isDone = false;
    while (!isDone) {
        isDone = true;  // Assume we are done unless an operation is performed
        for (size_t i = 0; i < exp.size(); i++) {
            string& x = exp.at(i);
            if (x == "-" || x == "+") {
                // Perform the operation
                double num = calc(stod(exp.at(i - 1)), stod(exp.at(i + 1)), x.at(0));
                exp[i - 1] = to_string(num);           // Replace left operand with result
                exp.erase(exp.begin() + i, exp.begin() + i + 2);  // Erase operator and right operand
                isDone = false;  // Another pass is needed
                break;
            }
        }
    }
}

int main(int argc, char* argv[]){
	if(argc == 1){
		cout << "Usage: " << argv[0] << "\"3*4\"";
		return 0;
	}

	vector<string> tokens = tokenize(argv[1]);

	logic(tokens);
	for(auto x: tokens)
		cout << x << endl;
	//toPrefix(argv[1]);

}

