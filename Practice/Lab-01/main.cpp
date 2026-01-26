#include <fstream>
#include <iostream>
#include <stack>
#include <string>

using namespace std;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;

    return 0;
}

int applyOper(int a, int b, char op, bool& error) {
    if (op == '+') return a + b;

    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') {
        if (b == 0) {
            error = true;
            return 1;
        }
        return a / b;
    }
    if (op == '%') {
        if (b == 0) {
            error = true;
            return 1;
        }
        return a % b;
    }

    return 0;
}

string evaluate(string equation) {
    /**
     * Initialize 2 Stack:
     * 1. Operator Stack
     * 2. Operand Stack
     */

    stack<int> operator_st;
    stack<int> values_st;

    bool error = false;

    for (int i = 0; i < equation.length(); i++) {
        if (equation[i] == ' ') continue;

        // Pushing ( to the operator_st
        if (equation[i] == '(') {
            operator_st.push(equation[i]);
        }

        else if (isdigit(equation[i])) {
            // Multi digit number like 150
            // Combining them into one single number

            int val = 0;
            while (i < equation.length() && isdigit(equation[i])) {
                val = (val * 10) + (equation[i] - '0');
                i++;
            }
            values_st.push(val);
            i--;
        }

        else if (equation[i] == ')') {
            while (!operator_st.empty() && operator_st.top() != '(') {
                int val2 = values_st.top();
                values_st.pop();

                int val1 = values_st.top();
                values_st.pop();

                char op = operator_st.top();
                operator_st.pop();

                values_st.push(applyOper(val1, val2, op, error));
            }
        }
    }
}

string readFromFile() {
    ifstream inputFile("./equations.txt");

    if (!inputFile.is_open()) {
        cout << "Error in opening file";
        return "";
    }

    string line;
    while (getline(inputFile, line)) {
    }

    return line;
}

int main() {
    // string equation = readFromFile();
    // cout << equation;

    // return 0;
}