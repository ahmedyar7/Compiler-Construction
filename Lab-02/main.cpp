#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

class Lexer {
   private:
    bool compare(const char* str1, const char* str2) {
        while (*str1 && *str2 && *str1 == *str2) {
            str1++;
            str2++;
        }
        return *str1 == *str2;
    }

    bool findKeywords(const char* ch) {
        const char* keywords[] = {
            "int", "main", "bool",    "float",  "double", "continue", "break",
            "if",  "else", "else if", "friend", "return", "break"};

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

        int i = 0;
        char ch;
        char buff[1024];

        while (inFile.get(ch)) {
            if (isspace(ch)) {
                continue;
            }

            if (isalpha(ch)) {
                i = 0;
                buff[i++] = ch;

                while (inFile.get(ch) && (isalpha(ch) || isdigit(ch))) {
                    buff[i++] = ch;
                }
                buff[i] = '\0';
                inFile.putback(ch);

                if (findKeywords(buff)) {
                    cout << "KEYWORDS: " << buff << endl;
                }

                else {
                    cout << "IDENTIFYER: " << buff << endl;
                }
            }

            else if (isdigit(ch)) {
                int i = 0;
                buff[i++] = ch;
                bool hasDot = false;

                while (inFile.get(ch) &&
                       (isdigit(ch) || (ch == '.' && !hasDot))) {
                    if (ch == '.') {
                        hasDot = true;
                    }
                    buff[i++] = ch;
                }
                buff[i] = '\0';
                inFile.putback(ch);

                cout << "Digit: " << ch << endl;

            }

            else if (ch == '<' || ch == '<=' || ch == '>' || ch == '>=') {
                char next;
                inFile.get(next);

                if (next == '=') {
                } else {
                }
            }

            else if (ch == '+' || ch == '-' || ch == '/' || ch == '*' ||
                     ch == '%') {
                cout << "OPERATOR: " << ch << endl;
            }

            else if (ch == '(' || ch == ')' || ch == '[' || ch == ']' ||
                     ch == '{' || ch == '}' || ch == ';' || ch == '.' ||
                     ch == ':') {
                cout << "DELIMITER: " << ch << endl;
            }
        }

        inFile.close();
        outFile.close();
    }
};

int main() {
    Lexer().scanner();

    return 0;
}