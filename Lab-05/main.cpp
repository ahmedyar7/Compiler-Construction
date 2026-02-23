#include <cctype>  // Required for isdigit, isalpha, isspace
#include <iostream>

using namespace std;

class Parser {
   private:
    const char* input;
    int pos;
    int depth;  // Used to visually indent the parse tree trace

    // Helper: Visually indent based on recursion depth
    void printIndent() {
        for (int i = 0; i < depth; ++i) cout << "  ";
    }

    // Helper: Skip whitespace to allow clean token reading
    void skipWhitespace() {
        while (input[pos] != '\0' && isspace(input[pos])) {
            pos++;
        }
    }

    // Helper: Look at the current character without consuming it
    char peek() {
        skipWhitespace();
        return input[pos];  // Returns '\0' if at the end of the character array
    }

    // Helper: Consume and return the current character
    char get() {
        skipWhitespace();
        if (input[pos] != '\0') return input[pos++];
        return '\0';
    }

    // Helper: Extract multi-digit numbers into a provided character buffer
    void matchNumber(char* buffer) {
        skipWhitespace();
        int i = 0;
        while (input[pos] != '\0' && isdigit(input[pos])) {
            buffer[i++] = input[pos++];
        }
        buffer[i] = '\0';  // Null-terminate the resulting character array
    }

    // Helper: Extract variable names into a provided character buffer
    void matchId(char* buffer) {
        skipWhitespace();
        int i = 0;
        while (input[pos] != '\0' && isalpha(input[pos])) {
            buffer[i++] = input[pos++];
        }
        buffer[i] = '\0';  // Null-terminate
    }

   public:
    // Constructor accepts a C-style string pointer
    Parser(const char* str) : input(str), pos(0), depth(0) {}

    // Main parsing entry point
    void parse() {
        cout << "Parsing: " << input << "\n";
        parseE();

        // If we finished parsing E but haven't reached the null terminator,
        // it's invalid
        if (peek() != '\0') {
            cout << "Syntax Error: Unexpected characters at the end: "
                 << &input[pos] << "\n";
        }
        
        else {
            cout << "Successfully parsed!\n";
        }
        cout << "------------------------------------------\n";
    }

    // E -> T { (+|-) T }
    void parseE() {
        printIndent();
        cout << "-> parseE()\n";
        depth++;

        parseT();
        while (peek() == '+' || peek() == '-') {
            char op = get();
            printIndent();
            cout << "Operator: " << op << "\n";
            parseT();
        }

        depth--;
        printIndent();
        cout << "<- parseE() return\n";
    }

    // T -> P { (*|/) P }
    void parseT() {
        printIndent();
        cout << "-> parseT()\n";
        depth++;

        parseP();
        while (peek() == '*' || peek() == '/') {
            char op = get();
            printIndent();

            cout << "Operator: " << op << "\n";
            parseP();
        }

        depth--;
        printIndent();
        cout << "<- parseT() return\n";
    }

    // P -> F [ ^ P ]
    void parseP() {
        printIndent();
        cout << "-> parseP()\n";
        depth++;

        parseF();

        if (peek() == '^') {
            char op = get();
            printIndent();

            cout << "Operator: " << op << "\n";
            parseP();  // Recursive call for right-associativity
        }

        depth--;
        printIndent();
        cout << "<- parseP() return\n";
    }

    // F -> -F | (E) | id | num
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
            get();  // Consume '('
            printIndent();

            cout << "Matched '('\n";
            parseE();

            if (peek() == ')') {
                get();  // Consume ')'
                printIndent();
                cout << "Matched ')'\n";
            } 
            
            else {
                printIndent();
                cout << "Syntax Error: Expected ')'\n";
            }
        }
        
        else if (isdigit(peek())) {
            char num[64];  // Fixed-size buffer for the number
            matchNumber(num);
            printIndent();
            cout << "Number: " << num << "\n";
        } 
        
        else if (isalpha(peek())) {
            char id[64];  // Fixed-size buffer for the identifier
            matchId(id);
            printIndent();
            cout << "Identifier: " << id << "\n";
        } 
        
        else {
            printIndent();
            cout << "Syntax Error: Unexpected token '" << peek() << "'\n";
        }

        depth--;
        printIndent();
        cout << "<- parseF() return\n";
    }
};

int main() {
    // 1. Unary & Precedence
    Parser test1("- 5 + 10 * 2");
    test1.parse();

    // 2. Right-Associative Power
    Parser test2("2 ^ 3 ^ 2");
    test2.parse();

    // 3. Complex Nesting
    Parser test3("-(5 + 2) ^ (10 / 5)");
    test3.parse();

    // 4. Evaluation Question Test
    Parser test4("5 ^ -2");
    test4.parse();

    return 0;
}