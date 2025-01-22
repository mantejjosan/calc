#include <iostream>
#include <filesystem>
#include <iomanip>
#include <yaml-cpp/yaml.h>
#include <map>
#include <stack>
#include <vector>
#include <queue>
#include <cmath>        // For fabs, powl
#include <iomanip>      // For setprecision, setw
#include <stdexcept>    // For runtime_error, invalid_argument
#include <limits>       // For numeric_limits
#include "filehandler.hpp"

using namespace std;
namespace fs = std::filesystem;

map<string, int> precedence = {
  {"^", 3},
  {"/", 2},
  {"*", 2},
  {"-", 1},
  {"+", 1},
  {"(", 0},  // Added precedence for left parenthesis
  {")", 0}  // Added precedence just to keep operator's existance record 
};

bool isDigit(char& suspect){
  return ((suspect >= '0' && suspect <= '9') || suspect == '.');
}

// Check if a character is an operator
bool isOperator(char c) {
  return precedence.find(string(1, c)) != precedence.end();
}

// Tokenize function
vector<string> tokenize(const string &exp) {
  vector<string> tokens;
  string num = "";
  int dotCount = 0;

  for (size_t i = 0; i < exp.size(); ++i) {
    char c = exp[i];

    if (isDigit(c)) {
      num += c;
      if (c == '.' && ++dotCount > 1) {
        cout << "Invalid Number: " << num << endl;
        return {};
      }
    } else {
      if (!num.empty()) {
        tokens.push_back(num);
        num = "";
        dotCount = 0;
      }

      if (c == ' ') {
        // Skip spaces
        continue;
      } else if (isOperator(c)) {
        // Handle implicit multiplication (e.g., 2(2) -> 2*(2))
        if (c == '(' && !tokens.empty() && (isDigit(tokens.back().back()) || tokens.back() == ")")) {
          tokens.push_back("*");
        }
        tokens.push_back(string(1, c));
      } else {
        cout << "Invalid character: " << c << endl;
        return {};
      }
    }
  }

  // Push the last number if any
  if (!num.empty()) {
    tokens.push_back(num);
  }

  return tokens;
}

bool isNumber(const string& suspect) {  // Changed to const reference
  if(suspect.empty()) return false;

  // Handle negative numbers
  int start = (suspect[0] == '-') ? 1 : 0;
  if(suspect.length() == start) return false;

  int dotcount = 0;
  for(size_t i = start; i < suspect.length(); i++) {
    if(suspect[i] == '.') dotcount++;
    else if(!(suspect[i] >= '0' && suspect[i] <= '9')) {
      return false;
    }
  }
  return dotcount <= 1;
}

// Shunting yard algorithm (produces postfix/RPN notation)
queue<string> getPostfix(vector<string>& tokens) {
  queue<string> output;
  stack<string> operators;
  
  for(const string& token : tokens) {
    if(isNumber(token)) {
      output.push(token);
    }
    else if(token == "(") {
      operators.push(token);
    }
    else if(token == ")") {
      // Check if stack is empty
      while(!operators.empty() && operators.top() != "(") {
        output.push(operators.top());
        operators.pop();
      }
      if(!operators.empty()) {
        operators.pop(); // Remove "("
      }
    }
    else { // Operator
      while(!operators.empty() && 
          operators.top() != "(" && 
          precedence[operators.top()] >= precedence[token]) {
        output.push(operators.top());
        operators.pop();
      }
      operators.push(token);
    }
  }

  // Pop remaining operators
  while(!operators.empty()) {
    if(operators.top() != "(") {
      output.push(operators.top());
    }
    operators.pop();
  }

  return output;
}

long double evaluatePostfix(queue<string> rpn) {
  stack<long double> values;

  while (!rpn.empty()) {
    string token = rpn.front();
    rpn.pop();

    // Try to convert to number
    try {
      size_t pos = 0;
      long double num = stold(token, &pos);
      if (pos == token.length()) {  // Ensure entire string was converted
        values.push(num);
        continue;
      }
    } catch (const invalid_argument& e) {
      // Not a number, continue to operator handling
    } catch (const out_of_range& e) {
      throw runtime_error("Number out of range: " + token);
    }

    // Handle operators
    if (values.size() < 2) {
      throw runtime_error("Invalid RPN expression: not enough operands");
    }

    long double b = values.top(); values.pop();
    long double a = values.top(); values.pop();

    if (token == "+") {
      values.push(a + b);
    } else if (token == "-") {
      values.push(a - b);
    } else if (token == "*") {
      values.push(a * b);
    } else if (token == "/") {
      if (fabs(b) < numeric_limits<long double>::epsilon()) {
        throw runtime_error("Division by zero");
      }
      values.push(a / b);
    } else if (token == "^") {
      // Handle special cases for power operation
      if (a == 0 && b < 0) {
        throw runtime_error("Zero raised to negative power");
      }
      if (a < 0 && floor(b) != b) {
        throw runtime_error("Negative number raised to non-integer power");
      }
      values.push(powl(a, b));
    } else {
      throw runtime_error("Unknown operator: " + token);
    }
  }

  if (values.size() != 1) {
    throw runtime_error("Invalid RPN expression: too many operands");
  }

  return values.top();
}


#include <iostream>
#include <string>
#include <vector>
#include <queue>

int main(int argc, char** argv) {
    std::string expression;
    std::vector<std::string> flags;

    // Parse command-line arguments
    for(int i = 1; i < argc; i++) {
        std::string param = argv[i];
        if (param[0] == '-') {
            flags.push_back(param);
        } else {
            expression += param;
        }
    }

    // Check if loading from history --in development
    Flaghandler handler(flags);
    if (std::find(flags.begin(), flags.end(), "-l") != flags.end()) {
        expression = handler.loadSession(0); // Load most recent session
    }

    try {
        std::vector<std::string> tokens = tokenize(expression);
        std::queue<std::string> postfix = getPostfix(tokens);
        long double result = evaluatePostfix(postfix);
        handler.print(result);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        handler.logError(e.what());
    }

    return 0;
}
