#include <cstring>
#include <iostream>

using namespace std;

char input[1000];
int pos = 0;

class SyntaxAnalysis {
   public:
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

        while (isdigit(peek())) {
            result = (result * 10) + (get() - '0');
            hasInteger = true;
        }

        if (peek() == '.') {
            get();
            while (isdigit(peek())) {
                fraction /= 10.0;
                result += (get() - '0') * fraction;
                hasInteger = true;
            }
        }

        if (!hasInteger) {
            error("not a number");
        }

        return result;
    }

    double factor() {
        if (peek() == '(') {
            get();
            double result = expression();

            if (peek() != ')') {
                error("Missing the parenthesis");
            }

            get();
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
                    error("Division by zero error...");
                }
                result = result / rhs;
            }
        }

        return result;
    }

   public:
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
};

int main() {
    SyntaxAnalysis sa = SyntaxAnalysis();

    cout << "Enter the expression: " << endl;

    cin.getline(input, 1000);

    try {
        double result = sa.expression();

        if (input[pos] != '\0') {
            sa.error("Unexpected error occured...");
        }

        cout << result << endl;
    }

    catch (const char* msg) {
        cout << msg << endl;
    }
    return 0;
}