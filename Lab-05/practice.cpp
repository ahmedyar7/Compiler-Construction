#include <cstring>
#include <iostream>

using namespace std;

class RecursiveDescentParser {
   private:
    int pos;
    const char* input;

    void skipWhitespaces() {
        while (input[pos] != '\0' && isspace(input[pos])) {
            pos++;
        }
    }

    char peek() {
        skipWhitespaces();
        return input[pos];
    }
    char get() {
        skipWhitespaces();
        return input[pos++];
    }

   public:
    RecursiveDescentParser(const char* str) : input(str), pos(0) {}

    void parse() {
        cout << "Reading Input: " << input << endl;
        parseE();
        cout << "\n===============================================\n";
        if (input[pos] == '\0') {
            cout << "Valid Expression: " << endl;
        } else {
            cout << "Invalid Expression: \n";
            cout << "Syntax Error: " << peek() << endl;
        }
    }
    void parseE() {
        parseT();
        while (peek() == '+' || peek() == '-') {
            cout << get() << " ";
            parseT();
        }
    }
    void parseT() {
        parseP();
        while (peek() == '*' || peek() == '/') {
            cout << get() << " ";
            parseP();
        }
    }
    void parseP() {
        parseF();
        if (peek() == '^') {
            cout << get() << " ";
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

            // ERROR MAY OCCUR
            if (get() != ')') {
                cout << "Error: Occured\n";
            }
        } else if (isalnum(c)) {
            while (isalnum(peek())) {
                cout << get() << endl;
            }
            cout << "[value,id]" << endl;
        } else {
            cout << "Error Occured " << peek() << endl;
        }
    }
};

int main() {
    RecursiveDescentParser("3 * (4 + 5) ^ 2").parse();
    return 0;
}