#include "RecursiveDescent.h"

#include <cctype>
#include <iostream>

using namespace std;

// Node* RecursiveDescent::parseE() {
//     Node* leftNode = parseT();  // Get the left side

//     while (peek() == '+' || peek() == '-') {
//         char op[2] = {get(), '\0'};
//         Node* newNode = new Node(NODE_OP, op);

//         newNode->left = leftNode;   // Old left side becomes left child
//         newNode->right = parseT();  // Get new right side

//         leftNode =
//             newNode;  // This node is now the "root" of this sub-expression
//     }
//     return leftNode;
// }

RecursiveDescent::RecursiveDescent()
    : input(nullptr), pos(0), hasError(false) {}

void RecursiveDescent::skipWhitespace() {
    while (input[pos] != '\0' && isspace(input[pos])) {
        pos++;
    }
}

char RecursiveDescent::peek() {
    skipWhitespace();
    return input[pos];
}

char RecursiveDescent::get() {
    skipWhitespace();
    return (input[pos] != '\0') ? input[pos++] : '\0';
}

void RecursiveDescent::parseExpression(const char* expression) {
    if (expression == nullptr || expression[0] == '\0') {
        return;
    }

    input = expression;
    pos = 0;
    hasError = false;

    cout << "Checking: " << expression << endl;
    parseE();

    if (!hasError && peek() == '\0') {
        cout << "Result: Valid Expression\n";
    }

    else {
        if (!hasError) {
            cout << "Result: Syntax Error (Unexpected characters at end)\n";
        }

        else {
            cout << "Result: Invalid Expression\n";
        }
    }
    cout << "--------------------------" << endl;
}

void RecursiveDescent::parseE() {
    parseT();
    while (peek() == '+' || peek() == '-') {
        get();

        parseT();
    }
}

void RecursiveDescent::parseT() {
    parseP();
    while (peek() == '*' || peek() == '/') {
        get();
        parseP();
    }
}

void RecursiveDescent::parseP() {
    parseF();
    if (peek() == '^') {
        get();
        parseP();
    }
}

void RecursiveDescent::parseF() {
    char c = peek();
    if (c == '-') {
        get();
        parseF();
    }

    else if (c == '(') {
        get();
        parseE();

        if (get() != ')') {
            cout << "Error: Missing ')'" << endl;
            hasError = true;
        }
    }

    else if (isalnum(c)) {
        while (isalnum(peek())) {
            get();
        }
    }

    else if (c == '\0') {
        cout << "Error: Unexpected End of Expression" << endl;
        hasError = true;
    }

    else {
        cout << "Error: Unexpected '" << c << "'" << endl;
        hasError = true;
        get();
    }
}