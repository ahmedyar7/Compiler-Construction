#include <iostream>
using namespace std;

char input[1000];
int pos = 0;

char peek() { return input[pos]; }
char get() { return input[pos++]; }
void error(const char* msg) { throw msg; }

double expression();
double term();
double factor();
double number();

double number() {
    double result = 0.0;
    double fraction = 1.0;
    bool hasInteger = false;
    bool hasDecimal = false;

    while (isdigit(peek())) {
        result = (result * 10) + (get() - '0');
        hasInteger = true;
    }

    if (isdigit(peek())) {
        fraction = fraction / 10.0;
        result += (get() - '0') * fraction;
        hasDecimal = true;
    }

    if (!hasDecimal) {
        error("Not a valid number");
    }

    return result;
}

double factor() {
    if (peek() == '(') {
        get();
        double result = expression();

        if (peek() != ')') {
            error("Parenthesis donot matches...");
        }

        get();  // getting the closing parenthsis
        return result;
    }

    return number();
}

double term() {
    double result = factor();

    while (peek() == '*' || peek() == '/') {
        char op = get();
        double rhs = factor();

        if (op == '*') {
            result *= rhs;
        }

        else if (op == '/') {
            if (rhs == 0) {
                error("Divsion by zero error");
            }
            result /= rhs;
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
            result += rhs;
        }

        else if (op == '-') {
            result -= rhs;
        }
    }

    return result;
}

int main() {
    cout << "Enter the Expression: ";
    cin.getline(input, 1000);

    try {
        double result = expression();

        if (input[pos] == '\0') {
            error("Unexpected form of error...");
        }
        cout << "Syntax Error: " << result << endl;
    }

    catch (const char* msg) {
        cout << msg << endl;
    }

    return 0;
}