#include <iostream>

using namespace std;

// ---------------------------------------------------------
// Global Variables (Replacing std::string)
// ---------------------------------------------------------
char input[1000];  // Fixed size character array buffer
int pos = 0;       // Current position in the array

// ---------------------------------------------------------
// Manual Helper Functions (Replacing library calls)
// ---------------------------------------------------------

// Manual implementation of isdigit()
bool is_digit(char c) { return c >= '0' && c <= '9'; }

// Function to calculate the length of a C-string manually
int str_len(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Get the current character without moving the position
char peek() { return input[pos]; }

// Get the current character and advance the position
char get() { return input[pos++]; }

// Custom error handling since we aren't using <stdexcept>
// We throw a const char* string literal directly
void error(const char* message) { throw message; }

// ---------------------------------------------------------
// Parser Functions
// ---------------------------------------------------------

// Forward declarations
double expression();
double term();
double factor();

// Manual implementation of number parsing (Replacing std::stod)
double number() {
    double result = 0.0;
    double fraction = 1.0;
    bool hasDigits = false;

    // Parse Integer part
    while (is_digit(peek())) {
        result = result * 10 + (get() - '0');
        hasDigits = true;
    }

    // Parse Decimal part
    if (peek() == '.') {
        get();  // Consume '.'
        while (is_digit(peek())) {
            fraction /= 10.0;
            result += (get() - '0') * fraction;
            hasDigits = true;
        }
    }

    if (!hasDigits) {
        // We handle string concatenation manually for the error message
        error("Expected a number");
    }

    return result;
}

// Factor -> NUMBER | '(' Expression ')'
double factor() {
    // Check for open parenthesis
    if (peek() == '(') {
        get();  // consume '('
        double result = expression();

        if (peek() != ')') {
            error("Missing closing parenthesis");
        }
        get();  // consume ')'
        return result;
    }

    // Otherwise it must be a number
    return number();
}

// Term -> Factor { ('*' | '/') Factor }
double term() {
    double result = factor();

    while (peek() == '*' || peek() == '/') {
        char op = get();
        double rhs = factor();

        if (op == '*') {
            result *= rhs;
        } else if (op == '/') {
            if (rhs == 0) error("Division by zero");
            result /= rhs;
        }
    }
    return result;
}

// Expression -> Term { ('+' | '-') Term }
double expression() {
    double result = term();

    while (peek() == '+' || peek() == '-') {
        char op = get();
        double rhs = term();

        if (op == '+') {
            result += rhs;
        } else {
            result -= rhs;
        }
    }
    return result;
}

// ---------------------------------------------------------
// Main
// ---------------------------------------------------------
int main() {
    cout << "Enter an expression (no spaces, e.g., 2+3*4): ";

    // Read input into char array
    cin.getline(input, 1000);

    try {
        double result = expression();

        // Check if we reached the end of the string (null terminator)
        if (input[pos] != '\0') {
            error("Unexpected character at end of input");
        }

        cout << "Valid syntax. Result = " << result << endl;

    } catch (const char* msg) {
        // Catch the string literal error message
        cout << "Syntax Error: " << msg << endl;
    }

    return 0;
}