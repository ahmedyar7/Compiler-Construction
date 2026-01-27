#include <cstring>
#include <fstream>
#include <iostream>
#include <stack>

using namespace std;

// CONSTANTS for buffer management
const int MAX_EXPR_BUFFER = 256;
const int MAX_RESULT_BUFFER = 64;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;

    return 0;
}

int applyOp(int a, int b, char op, bool& error) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b == 0) return error = true;
            return a / b;
        case '%':
            if (b == 0) return error = true;
            return a % b;
    }

    return 0;
}

void evalute(char* equation) {
    stack<int> values;
    stack<char> ops;

    bool error = false;
    int strLen = strlen(equation);

    for (int i = 0; i < strLen; i++) {

        if(equation[i] == ' ' || equation[i] == '\r' || equation[i] == '\n'){
            continue;
        }

        if(equation[i] == '('){
            ops.push(equation[i]);
        }

        else if(equation[i] == ')'){

            while(!ops.empty() && ops.top() != '('){
                
            }
        }
    }
}

void processFile() {
    ifstream inFile("./equations.txt");
    ofstream outFile("./results.txt");

    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "Failed to open the file \n";
        return;
    }

    char expr[MAX_EXPR_BUFFER];
    char resultStr[MAX_RESULT_BUFFER];

    while (inFile.getline(expr, MAX_RESULT_BUFFER)) {
        if (strlen(expr) <= 1) continue;

        outFile << expr << " = " << resultStr << endl;
    }
    cout << "finised writing content to file results.txt\n";
    return;
}

int main() { return 0; }
