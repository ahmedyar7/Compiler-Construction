#include <cstring>
#include <iostream>

using namespace std;

class RecursiveDescentParser {
   private:
    const char* input;
    int pos;

    void skipWhiteSpaces() {
        while (input[pos] != '\0' && isspace(input[pos])) {
            pos++;
        }
    }

    char peek() { return input[pos]; }
    char get() { return input[pos++]; }

   public:
    RecursiveDescentParser(const char* str) : input(str), pos(0) {}

    void parse() {
        cout << "Reading the input " << input << endl;
        parseE();

        if (peek() != '\0') {
            cout << "Valid Expression";
        } else {
            cout << "Invalid Expression " << endl;
        }
        cout << "\n+------------------------------------------+\n";
    }

    void parseE() {
        parseT();
        while (peek() == '+' || peek() == '-') {
            cout << "Operator" << get() << endl;
            parseT();
        }
    }

    void parseT() {
        parseP();
        while (peek() == '*' || peek() == '/') {
            cout << "Operator: " << get() << endl;
            parseP();
        }
    }
    void parseP() {
        parseF();
        if (peek() == '^') {
            cout << "Operator: " << get() << endl;
            parseP();
        }
    }
    void parseF() {
        char c = peek();
        if (c == '-') {
            get();
            parseF();
        } else if (c == '(') {
            get();
            parseE();
            if (get() != ')') {
                cout << "MisMatched Parenthesis" << endl;
            }
        } else if (isalnum(c)) {
            while (isalnum(peek())) {
                cout << get();
            }
            cout << "[value/id] " << endl;
        } else {
            cout << "Unexpected error occured\n";
        }
    }
};

int main() {
    RecursiveDescentParser("-5(10-2)+(20-0)").parse();

    return 0;
}