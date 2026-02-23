#include <cstring>
#include <iostream>

using namespace std;

class Parser {
   private:
    const char* input;
    int pos;
    int depth;

    void printIndent() {
        for (int i = 0; i < depth; i++) {
            cout << " ";
        }
    }

    void skipWhiteSpace() {
        while (input[pos] != '\0' && isspace(input[pos])) {
            pos++;
        }
    }

    char peek() {
        skipWhiteSpace();
        return input[pos];
    }

    char get() {
        skipWhiteSpace();
        if (input[pos] != '\0') {
            return input[pos++];
        }
        return '\0';
    }

    void matchNumber(char* buffer) {
        skipWhiteSpace();

        int i = 0;

        while (input[pos] != '\0' && isdigit(input[pos])) {
            buffer[i++] = input[pos++];
        }

        buffer[i] = '\0';
    }

    void matchId(char* buffer) {
        skipWhiteSpace();
        int i = 0;

        while (input[pos] != '\0' && isalpha(input[pos])) {
            buffer[i++] = input[pos++];
        }
        buffer[i] = '\0';
    }

   public:
    Parser(const char* str) : input(str), pos(0), depth(0) {}

    // main parsing entry point
    void parse() {
        cout << "Parsing " << input << endl;
        parseE();

        // if we have finished E but haven't reached the null terminator
        // it's invalid

        if (peek() != '0') {
            cout << "Syntax Error: Unexpected characters at the end: "
                 << &input[pos] << endl;
        }

        else {
            cout << "Successfully parsed!" << endl;
        }
        cout << "---------------------------\n";
    }

    // E -> T { (+|-) T}
    void parseE() {
        printIndent();
        cout << "-> parseE()" << endl;
        depth++;

        parseP();

        while (peek() == '+' || peek() == '-') {
            char op = get();
            printIndent();
            cout << "Operator: " << op << endl;
            parseP();
        }

        depth--;
        printIndent();
        cout << "<- parseE() return " << endl;
    }

    void parseP() {
        printIndent();
        cout << "-> parseP()" << endl;
        depth++;

        parseF();

        if (peek() == '^') {
            char op = get();
            printIndent();

            cout << "Operator: " << op << endl;
            parseP();
        }
        depth--;
        printIndent();
        cout << "<- parseP()" << endl;
    }

    void parseF() {
        printIndent();
        cout << "-> parseF()\n";
        depth++;

        if (peek() == '-') {
            char op = get();
            printIndent();
            cout << "Unary: " << op << "\n";
            parseF();
        }

        else if (peek() == '(') {
            get();
            printIndent();

            cout << "Matched '('\n";
            parseE();

            if (peek() == ')') {
                get();
                printIndent();
                cout << "Matched ')'\n";
            }

            else {
                printIndent();
                cout << "Syntax Error: Expected ')'" << endl;
            }
        }

        else if (isdigit(peek())) {
            char num[64];
            matchNumber(num);
            printIndent();
            cout << "number: " << num << endl;
        }

        else if (isalpha(peek())) {
            char id[64];
            matchId(id);
            printIndent();
            cout << "Identiyer: " << id << endl;
        }

        else {
            printIndent();
            cout << "Syntax Error: Unexpected Token: " << peek() << endl;
        }

        depth--;
        printIndent();
        cout << "<- parseF() return\n";
    }
};

int main() { return 0; }