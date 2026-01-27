#include "EqFileProcessor.h"

#include <fstream>
#include <iostream>

using namespace std;

int EqFileProcessor::getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    return 0;
}

int EqFileProcessor::applyOp(int a, int b, char op, bool& error) {
    if ((op == '/' || op == '%') && b == 0) {
        error = true;
        return 0;
    }
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return a / b;
    if (op == '%') return a % b;
    return 0;
}

// Name: Ahmed Yar
// CMS-ID: 480756
// Semester: 6th
// Course: Compile Construction
// Instructor: Dr. Adnan Idrees
// Lab-Engineer: Mr Safder Ali

void EqFileProcessor::evaluate(const char* tokens, ostream& output) {
    Stack<int> values;
    Stack<char> ops;
    bool error = false;

    for (int i = 0; tokens[i] != '\0'; i++) {
        if (tokens[i] == ' ') continue;

        if (tokens[i] == '(') {
            ops.push(tokens[i]);
        } else if (isdigit(tokens[i])) {
            int val = 0;
            while (tokens[i] != '\0' && isdigit(tokens[i])) {
                val = (val * 10) + (tokens[i] - '0');
                i++;
            }
            values.push(val);
            i--;
        } else if (tokens[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                int val2 = values.top();
                values.pop();
                int val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op, error));
            }
            if (!ops.empty()) ops.pop();
        } else {
            while (!ops.empty() &&
                   getPrecedence(ops.top()) >= getPrecedence(tokens[i])) {
                int val2 = values.top();
                values.pop();
                int val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op, error));
            }
            ops.push(tokens[i]);
        }
        if (error) {
            output << "Division by Zero";
            return;
        }
    }

    // Name: Ahmed Yar
    // CMS-ID: 480756
    // Semester: 6th
    // Course: Compile Construction
    // Instructor: Dr. Adnan Idrees
    // Lab-Engineer: Mr Safder Ali

    while (!ops.empty()) {
        int val2 = values.top();
        values.pop();
        int val1 = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOp(val1, val2, op, error));
        if (error) {
            output << "Division by Zero";
            return;
        }
    }

    output << values.top();
}

void EqFileProcessor::run(const char* inputPath, const char* outputPath) {
    ifstream inputFile(inputPath);
    ofstream outputFile(outputPath);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "File Error" << endl;
        return;
    }

    char expression[256];
    while (inputFile.getline(expression, 256)) {
        if (expression[0] == '\0') continue;
        outputFile << expression << " = ";
        evaluate(expression, outputFile);
        outputFile << endl;
    }

    inputFile.close();
    outputFile.close();
}