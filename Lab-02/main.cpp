#include <fstream>
#include <iostream>

using namespace std;

class LexicalAnalyzer {
   private:

    bool isAlpha(char ch){return (ch>='a'&&ch<='z') || (ch>='A'&&ch<='Z') || (ch=='_');}
    bool isDigit(char ch){return (ch>='0' && ch<='9');}
    bool isSpace(char ch){return (ch=='\n'||ch=='\r'||ch==' ');}

    bool compare(const char* str1, const char* str2) {
        while (*str1 && *str2 && *str1 == *str2) {
            str1++;
            str2++;
        }
        return *str1 == *str2;
    }

    bool checkKeyword(const char* ch) {
        const char* keywords[] = {"int",    "float",    "double", "bool",
                                  "true",   "void",     "auto",   "false",
                                  "return", "continue", "break"};

        for (int i = 0; i < sizeof(keywords) / sizeof(keywords[i]); i++) {
            if (compare(ch, keywords[i])) {
                return true;
            }
        }

        return false;
    }

   public:
    void scanner(const char* filename) {
        ifstream infile(filename);
        if (!infile) {
            return;
        }

        int i = 0;
        char buff[100];
        char ch;

        while (infile.get(ch)) {
            if (isSpace(ch)) {
                continue;
            }

            if (isAlpha(ch)) {
                i = 0;
                buff[i++] = ch;

                // using isdigit because the identifiyer can contains digits
                while (infile.get(ch) && (isAlpha(ch) || isDigit(ch))) {
                    buff[i++] = ch;
                }
                buff[i] = '\0';
                infile.putback(ch);

                if (checkKeyword(buff)) {
                    cout << "Keywords: " << buff << ", ";
                } 
                else {
                    cout << "Identifyers: " << buff << ", ";
                } 

                cout << endl;
                
                
            }

            // else if (isdigit(ch)) {
            //     i = 0;
            //     buff[100];

            //     bool hasDot = false;

            //     while (infile.get(ch) && (isdigit(ch)  || (ch == '.' && !hasDot))) {
            //         if (ch == '.') {
            //             hasDot = true;
            //         }

            //         buff[i++] = ch;
            //     }

            //     buff[i] = '\0';
            //     infile.putback(ch);

            //     cout << "Numbers: " << buff << ", ";
            // }

            // // managing the relation operators
            // else if (ch == '<' || ch == '>' || ch == '=' || ch == '!') {
            //     char next;
            //     infile.get(next);

            //     if (next == '=')
            //         cout << "relop: " << ch << "=" << endl;

            //     else {
            //         infile.putback(next);
            //         if (ch == '=')
            //             cout << "operator: =" << endl;
            //         else
            //             cout << "relop: " << ch << endl;
            //     }
            // }

            // // managing operators
            else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
                     ch == '%') {
                cout << "Operators: " << ch << " ";
            }

            // managing the delimiters
            else if (ch == '{' || ch == '}' || ch == '(' || ch == ')' ||
                     ch == '[' || ch == ']' || ch == ';' || ch == ':') {
                cout << "Delimitors: " << ch << ", ";
            }
            cout << endl;
        }

        infile.close();

        return;
    }
};

int main() {
    LexicalAnalyzer().scanner("./example.txt");

    return 0;
}