#include <fstream>
#include <iomanip>  // For table formatting
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class LexicalAnalyzer {
   private:
    // Helper to check if a string is a keyword
    bool isKeyword(const string& str) {
        static const string keywords[] = {
            "int",      "float", "double", "bool",     "include", "const",
            "void",     "true",  "false",  "unsigned", "main",    "return",
            "continue", "break", "cin",    "cout",     "using",   "namespace",
            "endl",     "std",   "if",     "else"};
        for (const string& kw : keywords) {
            if (str == kw) return true;
        }
        return false;
    }

    bool isAlpha(char ch) {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
               ch == '_';
    }
    bool isDigit(char ch) { return (ch >= '0' && ch <= '9'); }
    bool isSpace(char ch) {
        return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
    }

   public:
    void scanner(const string& inputFilename) {
        ifstream inFile(inputFilename);
        ofstream outFile("./output.txt");

        if (!inFile) {
            cerr << "ERROR: Could not open input file.\n";
            return;
        }

        // Write Table Header
        outFile << left << setw(20) << "TOKEN TYPE" << "| " << "LEXEME" << endl;
        outFile << string(40, '-') << endl;

        char ch;
        while (inFile.get(ch)) {
            if (isSpace(ch)) continue;

            // 1. IDENTIFIERS AND KEYWORDS
            if (isAlpha(ch)) {
                string buffer;
                buffer += ch;
                while (inFile.get(ch) && (isAlpha(ch) || isDigit(ch))) {
                    buffer += ch;
                }
                inFile.putback(ch);  // Put back the non-alpha char

                if (isKeyword(buffer))
                    writeRow(outFile, "KEYWORD", buffer);
                else
                    writeRow(outFile, "IDENTIFIER", buffer);
            }

            // 2. NUMBERS (Integers and Decimals)
            else if (isDigit(ch)) {
                string buffer;
                buffer += ch;
                bool hasDot = false;
                while (inFile.get(ch) &&
                       (isDigit(ch) || (ch == '.' && !hasDot))) {
                    if (ch == '.') hasDot = true;
                    buffer += ch;
                }
                inFile.putback(ch);
                writeRow(outFile, "NUMBER", buffer);
            }

            // 3. MULTI-CHARACTER OPERATORS (<<, >>, ==, !=, etc.)
            else if (string("+-*/%=!<>|&").find(ch) != string::npos) {
                string op;
                op += ch;
                char next;
                if (inFile.get(next)) {
                    // Check for combos: ==, !=, <=, >=, <<, >>, &&, ||
                    if ((ch == '<' && next == '<') ||
                        (ch == '>' && next == '>') || (next == '=') ||
                        (ch == '&' && next == '&') ||
                        (ch == '|' && next == '|')) {
                        op += next;
                    } else {
                        inFile.putback(next);
                    }
                }
                writeRow(outFile, "OPERATOR", op);
            }

            // 4. DELIMITERS / PUNCTUATION
            else if (string("(){}[],;.:#").find(ch) != string::npos) {
                string s;
                s += ch;
                writeRow(outFile, "DELIMITER", s);
            }
        }

        cout << "Lexical Analysis complete. Results saved to output.txt"
             << endl;
        inFile.close();
        outFile.close();
    }

   private:
    // Helper to format the table rows
    void writeRow(ofstream& out, const string& type, const string& lexeme) {
        out << left << setw(20) << type << "| " << lexeme << endl;
    }
};

int main() {
    LexicalAnalyzer lexer;
    lexer.scanner("example.txt");
    return 0;
}