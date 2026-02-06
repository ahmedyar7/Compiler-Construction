#include <cstdio>   // For snprintf
#include <cstring>  // For strlen
#include <fstream>
#include <iostream>
#include <stack>

using namespace std;

// Constants for buffer sizes
const int MAX_EXPR_SIZE = 256;
const int MAX_RES_SIZE = 64;

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
            return 0;
        }
        return a / b;
    }
    if (op == '%') {
        if (b == 0) {
            error = true;
            return 0;
        }
        return a % b;
    }
    return 0;
}

// Rewritten to use char arrays
void evaluate(const char* equation, char* resultBuffer) {
    stack<int> values;
    stack<char> ops;

    bool error = false;

    int len = strlen(equation);

    for (int i = 0; i < len; i++) {
        if (equation[i] == ' ' || equation[i] == '\r' || equation[i] == '\n')
            continue;

        if (isdigit(equation[i])) {
            int val = 0;
            while (i < len && isdigit(equation[i])) {
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

                values.push(applyOper(val1, val2, op, error));

                if (error) {
                    strcpy(resultBuffer, "Division Error Occurred");
                    return;
                }
            }

            if (!ops.empty()) ops.pop();
        }

        else {
            // Operator handling
            while (!ops.empty() &&
                   precedence(ops.top()) >= precedence(equation[i])) {
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOper(val1, val2, op, error));

                if (error) {
                    strcpy(resultBuffer, "Division Error Occurred");
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

        values.push(applyOper(val1, val2, op, error));

        if (error) {
            strcpy(resultBuffer, "Division Error Occurred");
            return;
        }
    }

    // Convert the final integer result to a char array
    if (!values.empty()) {
        snprintf(resultBuffer, MAX_RES_SIZE, "%d", values.top());
    }

    else {
        strcpy(resultBuffer, "0");
    }
}

void processFiles() {
    ifstream inFile("equations.txt");
    ofstream outFile("results.txt");

    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "File Error: Unable to open required files.\n";
        return;
    }

    char expr[MAX_EXPR_SIZE];
    char resultStr[MAX_RES_SIZE];

    // Using inFile.getline for C-style string reading
    while (inFile.getline(expr, MAX_EXPR_SIZE)) {
        if (strlen(expr) <= 1) continue;  // Skip empty/near-empty lines

        evaluate(expr, resultStr);
        outFile << expr << " = " << resultStr << endl;
    }

    inFile.close();
    outFile.close();

    cout << "Results written successfully to results.txt\n";
}

int main() {
    processFiles();
    return 0;
}