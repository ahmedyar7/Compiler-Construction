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
    // Practice Version
    stack<int> values;
    stack<char> ops;

    bool error = false;

    for (int i = 0; i < equation.length(); i++) {
        // handling whitespaces
        if (equation[i] == ' ') continue;

        // tokenization
        if (isdigit(equation[i])) {
            int val = 0;

            while (i < equation.length() && isdigit(equation[i])) {
                val = (val * 10) + (equation[i] - '0');
                i++;
            }
            values.push(val);
            i--;
        }

        // opening bracket
        else if (equation[i] == '(') {
            ops.push(equation[i]);
        }

        // calcuation till closing bracket is found
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
                    return "Division Error Occured";
                }
            }

            if (!ops.empty()) {
                ops.pop();
            }
        }

        // Checking the Precedence
        else {
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
                    return "Division Error Occured";
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

        if (error) return "Division Error Occured";
    }

    return to_string(values.top());
}

string readFromFile() {
    ifstream inFile("./equations.txt");
    ofstream outFile("results.txt");

    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "File Error: Unable to Open File equations.txt\n";
        return "";
    }

    string expr;

    while (getline(inFile, expr)) {
        if (expr.empty()) continue;

        string result = evaluate(expr);
        outFile << expr << " = " << result << endl;
    }

    inFile.close();
    outFile.close();

    cout << "Resuls of Expressing is Wrtting Successfully\n";
    cout << "Check: result.txt\n";

    return "";
}

int main() {
    readFromFile();

    // string equation = readFromFile();
    // cout << equation;

    return 0;
}