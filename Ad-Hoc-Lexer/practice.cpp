#include <cstring>
#include <fstream>
#include <iostream>
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
        const char* KEYWORDS[] = {"int",   "float",  "bool",     "double",
                                  "main",  "return", "continue", "true",
                                  "false", "class",  "include"};
        for (int i = 0; i < sizeof(KEYWORDS) / sizeof(KEYWORDS[i]); i++) {
            if (compare(KEYWORDS[i], kw)) {
                return true;
            }
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

        int i = 0;
        char ch;
        char buff[1024];

        while (Fin.get(ch)) {
            // Handling Whitespaces
            // - Space
            // - newline
            // - new cursor line
            if (isspace(ch)) {
                continue;
            }

            // Handling variables
            if (isalpha(ch)) {
                i = 0;
                buff[i++] = ch;
                while (Fin.get(ch) && (isdigit(ch) || isalpha(ch))) {
                    buff[i++] = ch;
                }
                buff[i] = '\0';
                Fin.putback(ch);

                if (findKeywords(buff)) {
                    cout << "KW: " << buff << endl;
                } else {
                    cout << "ID: " << buff << endl;
                }
            }

            if (isdigit(ch)) {
                i = 0;
                buff[i++] = ch;
                bool isDot = false;

                while (Fin.get(ch) && (isdigit(ch) || (ch == '.' && !isDot))) {
                    if (ch == '.') {
                        isDot = true;
                    }
                    buff[i++] = ch;
                }
                buff[i] = '\0';
                Fin.putback(ch);

                cout << "DIGITIS: " << buff << endl;
            }

            if (ch == '+' || ch == '-' || ch == '/' || ch == '%' || ch == '*') {
                cout << "ARITHEMATIC-OP: " << ch << endl;
            }

            if (ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' ||
                ch == '}') {
                cout << "DELIMITER: " << ch << endl;
            }

            if (ch == '<' || ch == '>' || ch == '>=' || ch == '<=' ||
                ch == '==') {
                char next = ch;
                Fin.get(next);
                if (next == '=') {
                    cout << "relop=" << ch << " = " << endl;
                } else {
                    if (ch == '=') {
                        cout << "RELATION-OP: " << ch << endl;
                    } else {
                        cout << "relop=" << ch << " = " << endl;
                    }
                }
            }

            if (ch == '||' || ch == '!' || ch == '&&') {
                cout << "Conditional_OP: " << ch << endl;
            }
        }
        return;
    }
};

int main() {
    AdHocLexer().lexer("./sample.txt");

    return 0;
}