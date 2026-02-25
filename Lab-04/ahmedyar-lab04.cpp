#include <iostream>
#include <cctype>

using namespace std;

class SimpleParser {
private:
    const char* input; // Use a pointer instead of std::string
    int pos;

    void skipWhitespace() {
        while (input[pos] != '\0' && isspace(input[pos])) pos++;
    }

    char peek() {
        skipWhitespace();
        return input[pos]; // C-strings naturally end with '\0'
    }

    char get() {
        skipWhitespace();
        return (input[pos] != '\0') ? input[pos++] : '\0';
    }

public:
    SimpleParser(const char* str) : input(str), pos(0) {}

    void parse() {
        cout << "Checking: " << input << endl;
        parseE();
        
        if (peek() == '\0') 
            cout << "Result: Valid Expression\n";
        else 
            cout << "Result: Syntax Error at '" << peek() << "'\n";
        cout << "--------------------------" << endl;
    }

    // E -> T { (+|-) T }
    void parseE() {
        parseT();
        while (peek() == '+' || peek() == '-') {
            cout << "Found Op: " << get() << endl;
            parseT();
        }
    }

    // T -> P { (*|/) P }
    void parseT() {
        parseP();
        while (peek() == '*' || peek() == '/') {
            cout << "Found Op: " << get() << endl;
            parseP();
        }
    }

    // P -> F [ ^ P ] (Right Associative Exponents)
    void parseP() {
        parseF();
        if (peek() == '^') {
            cout << "Found Op: " << get() << endl;
            parseP(); 
        }
    }

    // F -> -F | (E) | id | num
    void parseF() {
        char c = peek();
        if (c == '-') {
            get(); 
            parseF();
        } else if (c == '(') {
            get(); 
            parseE();
            if (get() != ')') cout << "Error: Missing ')'\n";
        } else if (isalnum(c)) {
            while (isalnum(peek())) cout << get(); 
            cout << " (Value/ID)" << endl;
        } else {
            cout << "Error: Unexpected '" << c << "'\n";
        }
    }
};

int main() {
    // You can pass raw string literals directly
    SimpleParser parser("3 * (4 + 5) ^ 2");
    parser.parse();

    return 0;
}