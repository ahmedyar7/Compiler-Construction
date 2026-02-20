#include <cstring>
#include <iostream>

using namespace std;

char inputBuff[100];
int pos = 0;

char peek() { return inputBuff[pos]; }
char get() { return inputBuff[pos++]; }
void error(const char* msg) { throw msg; }

double factor();
double term();
double expression();

// number -> decimals and fractions
double number() {
    double result = 0.0;
    double fraction = 1.0;

    bool hasDigit = false;
    bool hasFraction = false;

    while (isdigit(peek())) {
        result = (result * 10) + (get() - '0');
        hasDigit = true;
    }

    if (peek() == '.') {
        get();

        while (isdigit(peek())) {
            fraction = fraction / 10.0;
            result += (get() - '0') * fraction;
            hasFraction = false;
        }
    }

    if (!hasDigit) {
        error("Expected a number");
    }

    return result;
}

// factor -> parenthesis ( expression  )
double factor() {
    if (peek() == '(') {
        get();
        double result = expression();
        if (peek() != ')') {
            error("Missing matching parenthesis...");
        }
        get();
        return result;
    }
    return number();
}

// term -> * and /
double term() {
    double result = factor();

    while (peek() == '*' || peek() == '/') {
        char op = get();
        double rhs = factor();

        if (op == '*') {
            result = result * rhs;
        }

        else if (op == '/') {
            if (rhs == 0) {
                error("Division by zero error");
            }
            result = result / rhs;
        }
    }

    return result;
}

double expression() {
    double result = term();

    while (peek() == '+' || peek() == '-') {
        char op = get();
        double rhs = term();

        if (op == '+') {
            result = result + rhs;
        }

        else if (op == '-') {
            result = result - rhs;
        }
    }

    return result;
}

int main() {
    cout << "Enter the Expression: " << endl;
    cin.getline(inputBuff, 100);

    try {
        double result = expression();

        if (inputBuff[pos] == '\0') {
            error("Unexpected character at the end of input...");
        }

        cout << "Invalid Syntax = " << result << endl;
    }

    catch (const char* msg) {
        cout << "Syntax Error: " << msg << endl;
    }

    return 0;
}

// expr -> + and -