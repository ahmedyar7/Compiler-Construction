#include <cctype>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

class EquationProcessor {
   private:
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

    std::string evaluate(std::string tokens) {
        std::stack<int> values;
        std::stack<char> ops;
        bool error = false;

        for (int i = 0; i < tokens.length(); i++) {
            if (tokens[i] == ' ') continue;

            if (tokens[i] == '(') {
                ops.push(tokens[i]);
            } else if (isdigit(tokens[i])) {
                int val = 0;
                while (i < tokens.length() && isdigit(tokens[i])) {
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
                       precedence(ops.top()) >= precedence(tokens[i])) {
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
            if (error) return "Division by Zero";
        }

        while (!ops.empty()) {
            int val2 = values.top();
            values.pop();
            int val1 = values.top();
            values.pop();
            char op = ops.top();
            ops.pop();
            values.push(applyOp(val1, val2, op, error));
            if (error) return "Division by Zero";
        }

        return std::to_string(values.top());
    }

   public:
    void run(std::string inputPath, std::string outputPath) {
        std::ifstream inputFile(inputPath);
        std::ofstream outputFile(outputPath);

        if (!inputFile.is_open() || !outputFile.is_open()) {
            std::cerr << "File Error" << std::endl;
            return;
        }

        std::string expression;
        while (std::getline(inputFile, expression)) {
            if (expression.empty()) continue;
            std::string result = evaluate(expression);
            outputFile << expression << " = " << result << std::endl;
        }

        inputFile.close();
        outputFile.close();
    }
};

int main() {
    EquationProcessor processor;
    processor.run("equations.txt", "answers.txt");
    return 0;
}