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

void printResult(long double result) {
    // Save original formatting
    ios_base::fmtflags original_flags = cout.flags();
    streamsize original_precision = cout.precision();

    // Set maximum precision for long double
    int precision = numeric_limits<long double>::digits10 + 1;
    
    // Print in fixed decimal notation
    cout << "Result (fixed notation): " 
         << setprecision(precision) 
         << fixed 
         << result 
         << endl;
    
    // Print in scientific notation if number is very large or small
    if (abs(result) > 1e10 || (abs(result) < 1e-10 && result != 0)) {
        cout << "Result (scientific): " 
             << setprecision(precision) 
             << scientific 
             << result 
             << endl;
        
        // Show decimal places that are actually significant
        int significant_digits = floor(log10(abs(result)));
        if (significant_digits < 0) significant_digits = 0;
        cout << "Significant digits: " << significant_digits << endl;
    }
    
    // Print hexadecimal representation for debugging/verification
    cout << "Hexadecimal representation: " 
         << hex 
         << uppercase;
    
    // Since there's no direct way to print long double in hex,
    // we'll use a union to access its binary representation
    union {
        long double ld;
        unsigned char bytes[sizeof(long double)];
    } u;
    u.ld = result;
    
    for (size_t i = 0; i < sizeof(long double); ++i) {
        cout << setw(2) << setfill('0') 
             << static_cast<int>(u.bytes[i]);
    }
    cout << endl;
    
    // Restore original formatting
    cout.flags(original_flags);
    cout.precision(original_precision);
}

string findConfigFile(const string& filename) {
    fs::path currentPath = fs::current_path();
    fs::path configPath = currentPath / filename;
    
    // First try current directory
    if (fs::exists(configPath)) {
        return configPath.string();
    }
    
    // Try going up directories until we find the config file or hit root
    while (currentPath.has_parent_path()) {
        currentPath = currentPath.parent_path();
        configPath = currentPath / filename;
        
        if (fs::exists(configPath)) {
            return configPath.string();
        }
    }
    
    throw runtime_error("Could not find " + filename + " in project directory tree");
}

int main(int argc, char** argv) {
    string expression;
    vector<string> config;

    // Parse command-line arguments
    for(int i = 1; i < argc; i++) {
        string param = argv[i];
        if (param[0] == '-') {
            config.push_back(param); // Filter out all options
        } else {
            expression += param;    // Extract math expression
        }
    }

    // Load configuration from YAML file with root directory search
    YAML::Node yamlConfig;
    try {
        string configPath = findConfigFile("config.yaml");
        yamlConfig = YAML::LoadFile(configPath);
        // Optional: Print which config file we're using
        // cout << "Using config file: " << configPath << endl;
    } catch (const exception& e) {
        cerr << "Failed to load config.yaml: " << e.what() << endl;
        return 1;
    }

    // Parse YAML settings
    bool hex_output = yamlConfig["output_format"]["hex"].as<bool>(false);
    bool decimal_output = yamlConfig["output_format"]["decimal"].as<bool>(true);
    bool output_prompt = yamlConfig["output_prompt"].as<bool>(true);
    int precision = yamlConfig["precision"].as<int>(2); // Default precision: 2
    bool enable_gpu = yamlConfig["enable_gpu"].as<bool>(false);
    int pi_digits = yamlConfig["tasks"]["pi_digits"].as<int>(10); // Default to 10 digits

    // Tokenize the mathematical expression
    vector<string> tokens = tokenize(expression);

    // DEBUG: Print tokens
    /*
    cout << "Tokens formed:\n";
    for (auto x : tokens) cout << x << ", ";
    */

    // Convert infix to postfix
    queue<string> postfix = getPostfix(tokens);

    // DEBUG: Print postfix notation
    /*
    cout << "\nPostfix Notation:\n";
    queue<string> cppost = postfix;
    while (!cppost.empty()) {
        cout << cppost.front() << " ";
        cppost.pop();
    }
    */

    // Evaluate the postfix expression
    try {
        long double result = evaluatePostfix(postfix);

        // Output results based on YAML config
        if (output_prompt) {
            cout << "Result: ";
        }
        if (decimal_output) {
            cout << fixed << setprecision(precision) << result << endl;
        }
        if (hex_output) {
            cout << "Hexadecimal: " << hexfloat << result << endl;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
