#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

class AdHocLexer {
   private:
    bool isDigit(char ch) { return (ch >= '0' && ch <= '9'); }
    bool isAlpha(char ch) {
        return ch >= 'a' && ch <= 'z' || ch <= 'A' && ch == 'Z' || ch == '_';
    }
    bool isSpace(char ch) {
        return (ch == '\n' || ch == ' ' || ch == '\r' || ch == '\t');
    }

    bool compareStr(const char* str1, const char* str2) {
        while (*str1 && str2 && *str1 == *str2) {
            str1++;
            str2++;
        }
        return *str1 == *str2;
    }

    bool findKeywords(const char* kw) {
        const char* keywords[] = {"if",    "else",    "else if", "return",
                                  "true",  "false",   "bool",    "int",
                                  "float", "double",  "class",   "void",
                                  "char",  "private", "public",  "main"};

        for (int i = 0; i < sizeof(keywords) / sizeof(keywords[i]); i++) {
            if (compareStr(kw, keywords[i])) {
                return true;
            }
        }
        return false;
    }

   public:
    void scanner(const char* inputFile) {
        ifstream inFile(inputFile);

        if (!inFile) {
            cout << "File cannot be openend\n";
            return;
        }

        char ch;
        int i = 0;
        char buff[1024];

        while (inFile.get(ch)) {
            if (isSpace(ch)) {
                continue;
            }

            if (isAlpha(ch)) {
                i = 0;
                buff[i++] = ch;
                while (inFile.get(ch) && (isAlpha(ch) || isDigit(ch))) {
                    buff[i++] = ch;
                }
                buff[i] = '\0';
                inFile.putback(ch);
                if (findKeywords(buff)) {
                    cout << "KW: " << buff << endl;
                } else {
                    cout << "ID: " << buff << endl;
                }
            }

            if (isDigit(ch)) {
                i = 0;
                buff[i++] = ch;
                bool hasDot = false;

                while (inFile.get(ch) &&
                       (isDigit(ch) || (ch == '.' && !hasDot))) {
                    if (ch == '.') {
                        hasDot = true;
                    }
                    buff[i++] = ch;
                }
                buff[i] = '\0';
                inFile.putback(ch);
                cout << "DIGIT: " << buff << endl;
            }

            if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' ||
                ch == ']') {
                cout << "DELIMITER: " << ch << endl;
            }

            if (ch == '<' || ch == '<=' || ch == '>' || ch == '>=' ||
                ch == '==' || ch == '!=') {
                char next = ch;
                inFile.get(next);

                if (next == '=') {
                    cout << "relop: " << ch << " = " << endl;
                    if (ch == '=') {
                        cout << "RELATIONAL_OP: " << ch << endl;
                    } else {
                        cout << "OP: " << ch << endl;
                    }
                }
            }

            if (ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '%') {
                cout << "ARITHEMATIC_OP: " << ch << endl;
            }

            else {
                cout << "Unknown: " << buff << endl;
            }
        }

        inFile.close();
        return;
    }
};

int main() {
    AdHocLexer().scanner("input.txt");

    return 0;
}