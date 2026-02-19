#include <iostream>

using namespace std;

char inputBuff[100];
int pos = 0;

char peek() { return inputBuff[pos]; }
char get() { return inputBuff[pos++]; }

void error(const char* msg) { throw msg; }

// forward declearation
double expression();
double term();
double factor();

double number() {
    double fraction = 1.0;
    double result = 0.0;

    bool hasDecimal = false;
    bool hasInt = false;

    // parsing the digits
    while (isdigit(peek())) {
        result = (result * 10) + (get() - '0');
        hasDecimal = true;
    }

    // parsing the decimals
    if (peek() == '.') {
        get();  // getting the next

        while (isdigit(peek())) {
            fraction = fraction / 10.0;
            result = result + (get() - '0') * fraction;
            hasDecimal = true;
        }
    }

    // if not digits
    if (!hasDecimal) {
        error("Expected a number");
    }
    return result;
}

// Factor -> NUMBER | '(' expr ')'
double factor() {
    // checking the parenthesis
    if (peek() == '(') {
        get();
        double result = expression();

        if (peek() != ')') {
            error("Missing the closing paranthesis");
        }

        get();
        return result;
    }

    // if expr doesn't contain parenthesis then return
    // original expression...
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
                error("Division by zero error");
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
    cout << "Enter an expression (no spaces, e.g., 2+3*4): ";

    // Read input into char array
    cin.getline(inputBuff, 1000);

    try {
        double result = expression();

        // Check if we reached the end of the string (null terminator)
        if (inputBuff[pos] != '\0') {
            error("Unexpected character at end of input");
        }

        cout << "Valid syntax. Result = " << result << endl;

    } catch (const char* msg) {
        // Catch the string literal error message
        cout << "Syntax Error: " << msg << endl;
    }

    return 0;
}