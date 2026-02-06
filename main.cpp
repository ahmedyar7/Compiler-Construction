#include <cstring>
#include <fstream>
#include <iostream>
#include <stack>

using namespace std;

int const MAX_RESULT_SIZE = 64;
int const MAX_EXPR_SIZE = 256;

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

void calculate(char* eq, char* resultStr) {
    stack<int> values;
    stack<char> ops;

    int strLength = strlen(eq);
    bool error = false;

    for (int i = 0; i < strLength; i++) {
        if (eq[i] == '\n' || eq[i] == '\r' || eq[i] == ' ') {
            continue;
        }

        if (isdigit(eq[i])) {
            int val = 0;
            while (i < strLength && isdigit(eq[i])) {
                val = (val * 10) + (eq[i] - '0');
                i++;
            }
            values.push(val);
            i--;
        }

        else if (eq[i] == '(') {
            ops.push(eq[i]);
        }

        else if (eq[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                int val2 = values.top();
                values.pop();
                int val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();

                values.push(applyOps(val1, val2, op, error));

                if (error) {
                    strcpy(resultStr, "DIVISION ERROR");
                }
            }

            if(!ops.empty()) {
                ops.pop();
            }
        }

        else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(eq[i])) {
                int val2 = values.top();
                values.pop();
                int val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();

                values.push(applyOps(val1, val2, op, error));

                if (error) {
                    strcpy(resultStr, "DIVISON ERROR");
                }
            }

            ops.push(eq[i]);
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
            strcpy(resultStr, "DIVISION ERROR");
        }
    }

    if (!values.empty()) {
        snprintf(resultStr, MAX_RESULT_SIZE, "%d", values.top());
    } else {
        strcpy(resultStr, "0");
    }
}

void processFile() {
    ifstream inFile("./equations.txt");
    ofstream outFile("./results.txt");

    if (!inFile.is_open() && !outFile.is_open()) {
        cout << "---- FAILED TO OPEN THE equations.txt and results.txt FILE ---"
             << endl;
    }

    char expr[MAX_EXPR_SIZE];
    char resultStr[MAX_RESULT_SIZE];

    while (inFile.getline(expr, MAX_EXPR_SIZE)) {
        if (strlen(expr) < 1) continue;
        calculate(expr, resultStr);
        outFile << expr << " = " << resultStr << endl;
    }

    inFile.close();
    outFile.close();
    cout << "--- RESULTS ARE WRITTEN IN THE result.txt" << endl;
}

int main() {
    processFile();

    return 0;
}