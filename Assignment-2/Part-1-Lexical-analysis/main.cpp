#include <cstring>
#include <fstream>
#include <iostream>
#include <cctype> // for isalpha, isdigit, isspace

using namespace std;

class AdHocLexer {
private:
    bool compare(const char* str1, const char* str2) {
        while (*str1 && *str2 && *str1 == *str2) {
            str1++;
            str2++;
        }
        return *str1 == *str2;
    }

    bool findKeywords(const char* kw) {
        const char* KEYWORDS[] = {"int", "float", "bool", "double",
                                  "main", "return", "continue", "true",
                                  "false", "class", "include"};
        for (int i = 0; i < sizeof(KEYWORDS) / sizeof(KEYWORDS[0]); i++) {
            if (compare(KEYWORDS[i], kw)) return true;
        }
        return false;
    }

public:
    void lexer(const char* fileName) {
        ifstream Fin(fileName);
        if (!Fin) {
            cout << "Unable to open the file...\n";
            return;
        }

        char ch;
        char buff[1024];

        while (Fin.get(ch)) {
            // 1. Handling Whitespaces
            if (isspace(ch)) continue;

            // 2. Handling Keywords and Identifiers
            if (isalpha(ch)) {
                int i = 0;
                buff[i++] = ch;
                while (Fin.get(ch) && (isalnum(ch))) {
                    buff[i++] = ch;
                }
                buff[i] = '\0';
                Fin.putback(ch); // Put back the non-alpha char

                if (findKeywords(buff)) cout << "KW: " << buff << endl;
                else cout << "<ID, " << buff << ">" << endl;
            }

            // 3. Handling Numbers
            else if (isdigit(ch)) {
                int i = 0;
                buff[i++] = ch;
                bool isDot = false;
                while (Fin.get(ch) && (isdigit(ch) || (ch == '.' && !isDot))) {
                    if (ch == '.') isDot = true;
                    buff[i++] = ch;
                }
                buff[i] = '\0';
                Fin.putback(ch); 
                cout << "<NUM, " << buff << ">" << endl;
            }

            // 4. Handling Arithmetic Operators
            else if (ch == '+' || ch == '-' || ch == '/' || ch == '%' || ch == '*') {
                if (ch == '+') cout << "<PLUS, +>" << endl;
                else if (ch == '-') cout << "<MINUS, ->" << endl;
                else if (ch == '*') cout << "<MULT, *>" << endl;
                else if (ch == '/') cout << "<DIV, />" << endl;
                else if (ch == '%') cout << "<MOD, %>" << endl;
            }

            // 5. Handling Delimiters
            else if (ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}') {
                if (ch == '(') cout << "<LPARN, (>" << endl;
                else if (ch == ')') cout << "<RPARN, )>" << endl;
                else cout << "<DELIM, " << ch << ">" << endl;
            }

            // 6. Handling Relational and Logical Operators
            else if (ch == '<' || ch == '>' || ch == '=' || ch == '!' || ch == '&' || ch == '|') {
                char next;
                Fin.get(next);

                if (ch == '=' && next == '=') cout << "<RELOP, ==>" << endl;
                else if (ch == '!' && next == '=') cout << "<RELOP, !=>" << endl;
                else if (ch == '<' && next == '=') cout << "<RELOP, <=>" << endl;
                else if (ch == '>' && next == '=') cout << "<RELOP, >=>" << endl;
                else if (ch == '&' && next == '&') cout << "<LOGICOP, &&>" << endl;
                else if (ch == '|' && next == '|') cout << "<LOGICOP, ||>" << endl;
                else {
                    // It was a single character operator
                    Fin.putback(next);
                    if (ch == '=') cout << "<ASSIGN, =>" << endl;
                    else if (ch == '!') cout << "<NOT, !>" << endl;
                    else cout << "<RELOP, " << ch << ">" << endl;
                }
            }
        }
    }
};

int main() {
    AdHocLexer().lexer("./sample.txt");
    return 0;
}