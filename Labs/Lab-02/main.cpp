#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

class Lexer {
   private:
    bool isAlpha(char ch) {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
               (ch == '_');
    }
    bool isDigit(char ch) { return ch >= '0' && ch <= '9'; }
    bool isSpace(char ch) { return (ch == ' ' || ch == '\n' || ch == '\r'); }

    bool compare(const char* str1, const char* str2) {
        while (*str1 && *str2 && *str1 == *str2) {
            str1++;
            str2++;
        }

        return *str1 == *str2;
    }

    bool findKeywords(char* ch) {
        const char* keywords[] = {"int",    "float",  "double",   "char",
                                  "void",   "cin",    "cout",     "bool",
                                  "friend", "return", "continue", "break",
                                  "if",     "else",   "else if"};

        for (int i = 0; i < sizeof(keywords) / sizeof(keywords[i]); i++) {
            if (compare(ch, keywords[i])) {
                return true;
            }
        }

        return false;
    }

   public:
    void scanner() {
        ifstream inFile("./example.txt");
        ofstream outFile("./output.txt");

        if (!inFile) {
            cout << "Unable to open the example.txt file...";
            return;
        }

        int i = 0;
        char ch;
        char buff[1024];

        while (inFile.get(ch)) {
            if (isSpace(ch)) {
                continue;
            }

            if (isAlpha(ch)) {
                i = 0;
                buff[i++] = ch;

                while (inFile.get(ch) && (isDigit(ch) || isAlpha(ch))) {
                    buff[i++] = ch;
                }

                buff[i] = '\0';
                inFile.putback(ch);

                if (findKeywords(buff))
                    // cout << "Value:\n" << buff << "\tType" << "Keyword\n";
                cout << "Keyword: " << buff << endl;

                else
                    cout << "Identifyers: " << buff << endl;
            }

            else if (isDigit(ch)) {
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

                buff[i++] = '\0';
                inFile.putback(ch);

                cout << "DIGITS: " << buff << endl;
            }

            else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
                     ch == '%') {
                cout << "OPERATORS: " << ch << endl;
            }

            else if (ch == '(' || ch == ';' || ch == '.' || ch == ':' ||
                     ch == ')' || ch == '{' || ch == '}' || ch == '[' ||
                     ch == ']') {
                cout << "DELIMITER: " << ch << endl;
            }
        }
    }
};

int main() {
    Lexer().scanner();
    return 0;
}