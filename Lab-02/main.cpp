#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class LexicalAnalyzer {
   private:
    vector<string> KEYWORDS;
    vector<string> INDENTIFYERS;
    vector<string> DIGITS;

    vector<char> OPERATORS;
    vector<char> DELIMITERS;

    bool compare(const char* str1, const char* str2) {
        while (*str1 && *str2 && *str1 == *str2) {
            str1++;
            str2++;
        }

        return *str1 == *str2;
    }

    bool findKeywords(char* ch) {
        const char* keywords[] = {"int",    "float", "double",   "char",
                                  "void",   "true",  "false",    "bool",
                                  "return", "break", "continue", "main"};

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

        if (!inFile) {
            cout << "ERROR: Failed to open the file" << endl;
        }

        char ch;
        int i = 0;
        char buff[100];

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
                    KEYWORDS.push_back(buff);
                    // cout << "< Keyword, " << buff << " >" << endl;
                } else {
                    INDENTIFYERS.push_back(buff);
                    // cout << "< Idenifyer: " << buff << " >" << endl;
                }
            }

            else if (isdigit(ch)) {
                i = 0;
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

                DIGITS.push_back(buff);
                // cout << "< Digit: " << buff << " >" << endl;
            }

            else if (ch == '+' || ch == '-' || ch == '/' || ch == '%' ||
                     ch == '^' || ch == '*') {
                OPERATORS.push_back(ch);
            }

            else if (ch == '[' || ch == ']' || ch == ';' || ch == '.' ||
                     ch == '{' || ch == '}' || ch == '(' || ch == ')') {
                DELIMITERS.push_back(ch);
            }
        }

        inFile.close();

        ofstream Fout("output.txt");

        // all keywords;
        Fout << "KEYWORDS: " << "{";
        for (auto keys : KEYWORDS) {
            Fout << keys << "  ";
        }
        cout << "}" << endl;
        

        // all INDENTIFYERS
        Fout << "IDENTIFYERS: " << "{";
        for (auto keys : INDENTIFYERS) {
            Fout << keys << " ";
        }
        Fout << "}" << endl;

        // all digits
        Fout << "DIGITS: " << "{";
        for (auto keys : DIGITS) {
            Fout << keys << " ";
        }
        Fout << "}" << endl;

        Fout << "OPERATORS: " << "{";
        for (auto keys : OPERATORS) {
            Fout << keys << " ";
        }
        Fout << "}" << endl;

        Fout << "DELIMITERS: " << "{";
        for (auto keys : DELIMITERS) {
            Fout << keys << " ";
        }
        Fout << "}" << endl;
    }
};

int main() {
    LexicalAnalyzer().scanner();

    return 0;
}