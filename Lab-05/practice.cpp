#include <iostream>

using namespace std;

class SimpleParser {
   private:
    const char* input;
    int pos;

    void skipWhiteSpaces() {
        while (input[pos] != '\0' && isspace(input[pos])) {
            pos++;
        }
    }

    char peek() {
        skipWhiteSpaces();
        return input[pos];
    }

    char get() {
        skipWhiteSpaces();
        return input[pos++];
    }

   public:
    SimpleParser(const char* str) : input(str), pos(0) {}

    // void parse();   // parse -> parseE()
    // void parseE();  // parseE()->parseT() {+|-}
    // void parseT();  // parseT()->parseP() {*|/}
    // void parseP();  // parseP()->parseF() {^}
    // void parseF();  // parseF()

    void parse() {
        cout << "Checking: " << input << endl;
        parseE();

        if (peek() == '\0') {
            cout << "Result: Valid Expression\n";
        } else {
            cout << "Result: Syntax Error\n" << peek() << "\n";
        }
    }

    void parseE() {
        parseT();

        while (peek() == '+' || peek() == '-') {
            cout << "Found OP: " << get() << endl;
            parseT();
        }
    }

    void parseT() {
        parseP();

        while (peek() == '*' || peek() == '/') {
            cout << "Found Op" << get() << endl;
            parseP();
        }
    }

    void parseP() {
        parseF();

        while (peek() == '^') {
            cout << "Found OP:" << get() << endl;
            parseF();
        }
    }

    void parseF() {
        char c = peek();

        if (c == '-') {
            get();
            parse();
        }

        else if (c == '(') {
            get();
            parseE();  // {+|-}

            if (get() != ')') {
                cout << "Error: Missing )" << endl;
            }
        }

        else if (isalnum(c)) {
            while (isalnum(peek())) {
                cout << get();
            }

            cout << "(Value/ID) " << endl;
        }

        else {
            cout << "Error: Unexpected " << c << endl;
        }
    }
};

int main() {
    // You can pass raw string literals directly
    SimpleParser parser("3 * (4 + 5) ^ 2");
    parser.parse();

    return 0;
}