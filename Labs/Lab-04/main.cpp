#include <iostream>
using namespace std;

char input[1024];
int pos = 0;

char peek() { return input[pos]; }
char get() { return input[pos++]; }
void error(const char* msg) { throw msg; }

double expression();
double term();
double factor();
double number();

double number() {
    bool hasInt = false;
    double result = 0.0;
    double fraction = 1.0;

    while (isdigit(peek())) {
        result = (result * 10) + (get() - '0');
        hasInt = true;
    }

    if (peek() == '.') {
        get();

        while (isdigit(peek())) {
            fraction = fraction / 10.0;
            result += (get() - '0') * fraction;
            hasInt = true;
        }
    }

    if (!hasInt) {
        error("Not a number");
    }
    return result;
}

double factor() {
    if (peek() == '(') {
        get();

        double expr = expression();
        if (peek() != ')') {
            error("Missing the matching parentheisis...");
        }
        
        get();
        return expr;
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

        if (op == '/') {
            if (rhs == 0) {
                error("Division in the error");
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
        if (op == '-') {
            result -= rhs;
        }
    }
    return result;
}

int main() {
    cout << "Enter the Expression: " << endl;

    try {
        cin.getline(input, 1024);

        double result = expression();

        if (input[pos] != '\0') {
            error("Unexpected error occurred...");
        }
        cout << result << endl;

    } catch (const char* error) {
        cout << error << endl;
    }

    return 0;
}