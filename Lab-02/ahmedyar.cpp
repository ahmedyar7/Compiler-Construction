#include <fstream>
#include <iostream>

using namespace std;

bool isDigit(char ch) { return ch >= '0' && ch <= '9'; }

bool isAlpha(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

bool isSpace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

bool compare(const char* s1, const char* s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }

    return *s1 == *s2;
}

// Name: Ahmed Yar
// CMS-ID: 480756

bool checkKeyword(const char* buffer) {
    const char* keywords[] = {"while",    "if",  "else",  "return", "break",
                              "continue", "int", "float", "void",   "for"};

    for (int i = 0; i < 10; i++) {
        if (compare(buffer, keywords[i])) return true;
    }

    return false;
}

void scanner(const char* filename) {
    ifstream fin(filename);
    if (!fin) return;

    char ch;
    char buffer[100];

    int i;

    while (fin.get(ch)) {
        if (isSpace(ch)) continue;

        if (isAlpha(ch)) {
            i = 0;
            buffer[i++] = ch;

            while (fin.get(ch) && (isAlpha(ch) || isDigit(ch))) {
                buffer[i++] = ch;
            }

            buffer[i] = '\0';
            fin.putback(ch);

            if (checkKeyword(buffer))
                cout << "keyword : " << buffer << endl;

            else
                cout << "identifier: " << buffer << endl;

        }

        // Name: Ahmed Yar
        // CMS-ID: 480756
        else if (isDigit(ch)) {
            i = 0;
            buffer[i++] = ch;

            bool hasDot = false;

            while (fin.get(ch) && (isDigit(ch) || (ch == '.' && !hasDot))) {
                if (ch == '.') hasDot = true;
                buffer[i++] = ch;
            }

            buffer[i] = '\0';
            fin.putback(ch);

            cout << "num: " << buffer << endl;
        }

        else if (ch == '<' || ch == '>' || ch == '!' || ch == '=') {
            char next;
            fin.get(next);

            if (next == '=')
                cout << "relop: " << ch << "=" << endl;

            else {
                fin.putback(next);
                if (ch == '=')
                    cout << "operator: =" << endl;
                else
                    cout << "relop: " << ch << endl;
            }

        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            cout << "operator: " << ch << endl;

        }

        // Name: Ahmed Yar
        // CMS-ID: 480756

        else if (ch == '(' || ch == ')' || ch == '{' || ch == '}' ||
                 ch == '[' || ch == ']' || ch == ',' || ch == ';') {
            cout << ch << ":" << ch << endl;
        }
    }
    fin.close();
}

int main() {
    scanner("./example.txt");  // <<<<< INDENTIFY YOUR FILE HERE

    return 0;
}