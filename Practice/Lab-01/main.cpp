#include <cstring>
#include <fstream>
#include <iostream>
#include <stack>

using namespace std;

const int RESULTS_STRING_BUFFER = 64;
const int EXPRESSION_BUFFER = 256;

class ProcessEquation {
   private:
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
                }
                return a / b;

            case '%':
                if (b == 0) {
                    error = true;
                }
                return a % b;
        }

        return 0;
    }

    void evaluate(char* eq, char* resultStr) {
        stack<int> values;
        stack<char> ops;

        int strLen = strlen(eq);
        bool error = false;

        for (int i = 0; i < strLen; i++) {
            if (eq[i] == ' ' || eq[i] == '\r' || eq[i] == '\n') {
                continue;
            }

            if (isdigit(eq[i])) {
                int val = 0;
                while (i < strLen && isdigit(eq[i])) {
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
                        strcpy(resultStr, "Division Error");
                        return;
                    }
                }

                if (!ops.empty()) {
                    ops.pop();
                }
            }

            else {
                while (!ops.empty() &&
                       precedence(ops.top()) >= precedence(eq[i])) {
                    int val2 = values.top();
                    values.pop();

                    int val1 = values.top();
                    values.pop();

                    char op = ops.top();
                    ops.pop();

                    values.push(applyOps(val1, val2, op, error));

                    if (error) {
                        strcpy(resultStr, "DIVISION ERROR");
                        return;
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
                strcpy(resultStr, "Division error");
                return;
            }
        }

        if (!values.empty()) {
            snprintf(resultStr, RESULTS_STRING_BUFFER, "%d", values.top());
            return;
        }

        else {
            strcpy(resultStr, "0");
            return;
        }
    }

   public:
    void processFile() {
        ifstream inFile("./equations.txt");
        ofstream outFile("./results.txt");

        if (!inFile.is_open() || !outFile.is_open()) {
            cout << "Error in Opening and closing of file" << endl;
            return;
        }

        char expr[EXPRESSION_BUFFER];
        char resultStr[RESULTS_STRING_BUFFER];

        while (inFile.getline(expr, EXPRESSION_BUFFER)) {
            if (strlen(expr) < 1) continue;

            evaluate(expr, resultStr);
            outFile << expr << " = " << resultStr << endl;
        }

        inFile.close();
        outFile.close();

        cout << "\n--- FINISHED WRITING THE THE FILE (results.txt) ---\n";

        return;
    }
};

int main() {
    ProcessEquation().processFile();

    return 0;
}