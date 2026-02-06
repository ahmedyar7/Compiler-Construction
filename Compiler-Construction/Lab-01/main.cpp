#include <cstring>
#include <fstream>
#include <iostream>
#include <stack>

using namespace std;

const int MAX_EXPRESSION_SIZE = 256;
const int MAX_RESULT_SIZE = 64;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;

    return 0;
}

int applyOps(int a, int b, char op, bool& error) {
    switch (op) {
        case '+':
            return a + b;

        case '-':
            return a - b;

        case '*':
            return a * b;

        case '/':

            if (b == 0) {
                error = true;
                return 0;
            }
            return a / b;

        case '%':
            if (b == 0) {
                error = true;
                return 0;
            }
            return a % b;
    }

    return 0;
}

void evaluateEquation(char* equation, char* resultStr) {
    stack<int> values;
    stack<char> ops;

    bool error = false;

    int strLen = strlen(equation);

    for (int i = 0; i < strLen; i++) {
        if (equation[i] == ' ' || equation[i] == '\r' || equation[i] == '\n') {
            continue;
        }

        if (isdigit(equation[i])) {
            int val = 0;

            while (i < strLen && isdigit(equation[i])) {
                val = (val * 10) + (equation[i] - '0');
                i++;
            }

            values.push(val);
            i--;
        }

        else if (equation[i] == '(') {
            ops.push(equation[i]);
        }

        else if (equation[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOps(val1, val2, op, error));

                if (error) {
                    strcpy(resultStr, "Division by Zero Error Message");
                    return;
                }
            }

            if (!ops.empty()) {
                ops.pop();
            }
        }

        else {
            while (!ops.empty() &&
                   precedence(ops.top()) >= precedence(equation[i])) {
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOps(val1, val2, op, error));

                if (error) {
                    strcpy(resultStr, "Division by Zero Error Message");
                    return;
                }
            }

            ops.push(equation[i]);
        }
    }

    while (!ops.empty()) {
        int val2 = values.top();
        values.pop();

        int val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOps(val1, val2, op, error));

        if (error) {
            strcpy(resultStr, "Division by zero error");
            return;
        }
    }

    if (!values.empty()) {
        snprintf(resultStr, MAX_RESULT_SIZE, "%d", values.top());
    }

    else {
        strcpy(resultStr, "0");
        return;
    }
}

void processFile() {
    ifstream inFile("./equations.txt");
    ofstream outFile("./results.txt");

    if (!inFile.is_open() && !outFile.is_open()) {
        cout << "Failed to open input and output file";
        return;
    }

    char expr[MAX_EXPRESSION_SIZE];
    char resultString[MAX_RESULT_SIZE];

    while (inFile.getline(expr, MAX_EXPRESSION_SIZE)) {
        if (strlen(expr) <= 1) continue;

        evaluateEquation(expr, resultString);

        outFile << expr << " = " << resultString << endl;
    }

    inFile.close();
    outFile.close();

    cout << "\n--- Finish Writing to the results.txt file ----\n";
}

int main() {
    processFile();

    return 0;
}