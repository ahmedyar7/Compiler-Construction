#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

class AdHocLexer {
   private:
    bool compareStr(const char* str1, const char* str2) {
        while (*str1 && *str2 && *str1 == *str2) {
            str1++;
            str2++;
        }
        return *str1 == *str2;
    }

    bool findKeywords(const char* kw) {
        const char* KEYWORDS[] = {"int",  "main",  "float",    "double", "bool",
                                  "true", "false", "continue", "return", "char",
                                  "if",   "else",  "else if"};

        for (int i = 0; i < sizeof(KEYWORDS) / sizeof(KEYWORDS[i]); i++) {
            if (compareStr(KEYWORDS[i], kw)) {
                return true;
            }
        }
        return false;
    }

   public:
    void lexer(const char* fileName) {
        ifstream inFile("./sample.txt");

        if (!inFile) {
            cout << "ERROR: File (sample.txt) cannot be openned\n";
            return;
        }

        int i = 0;
        char ch;
        char buff[100];

        while (inFile.get(ch)) {
            // Managing the Spaces
            // - whitespaces,
            // - newline
            // - next line cursor
            if (isspace(ch)) {
                continue;
            }

            // Managing the AlphaNumeric Identifyers
            // - userdefined variables, constants
            if (isalpha(ch)) {
                i = 0;
                buff[i++] = ch;

                while (inFile.get(ch) && (isalpha(ch) || isdigit(ch))) {
                    buff[i++] = ch;
                }
                buff[i] = '\0';

                if (findKeywords(buff)) {
                    cout << "KW: " << buff << endl;
                } else {
                    cout << "ID: " << buff << endl;
                }
            }

            // Managing the Digits(int and float/double)
            else if (isdigit(ch)) {
                i = 0;
                buff[i++] = ch;
                bool hasDot = false;

                while (inFile.get(ch) && (isdigit(ch)) ||
                       (ch == '.' && !hasDot)) {
                    if (ch == '.') {
                        hasDot = true;
                    }
                    buff[i++] = ch;
                }

                buff[i] = '\0';
                cout << "DIGITS: " << buff << endl;
            }

            // Managing relational operators
            // < > == <= >= !=
            else if (ch == '<' || ch == '<=' || ch == '>' || ch == '>=' ||
                     ch == '==' || ch == '!=') {
                char next = ch;
                inFile.get(next);

                if (next == '=') {
                    cout << "relop: " << ch << " = " << endl;
                } else {
                    if (ch == '=') {
                        cout << "Relation-Operator: " << ch << endl;
                    } else {
                        cout << "relop: " << ch << " = " << endl;
                    }
                }
            }

            // Managing the Operators
            // + - / * %
            else if (ch == '+' || ch == '-' || ch == '/' || ch == '*') {
                char next = ch;
                inFile.get(ch);

                if (next == '=') {
                    cout << "relop: " << ch << " = " << endl;
                } else {
                    if (ch == '=') {
                        cout << "Relation-Operator: " << ch << endl;
                    } else {
                        cout << "relop: " << ch << " = " << endl;
                    }
                }
            }

            // Manging the Delimiters
            // - () {} [] : .
            else if (ch == '(' || ch == ')' || ch == '[' || ch == ']' ||
                     ch == '{' || ch == '}' || ch == ';' || ch == '.' ||
                     ch == '.') {
                cout << "DELIMITERS: " << ch << endl;
            }

            // Manging the Conditional Operators
            // || && !
            else if (ch == '||' || ch == '&&' || ch == '!') {
                cout << "CONDITIONAL-OPERATOR: " << ch << endl;
            }
        }
    }
};

int main() {
    AdHocLexer().lexer("./sample.txt");
    return 0;
}