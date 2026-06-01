#include "AdHocLexer.h"

#include <cctype>
#include <cstring>

using namespace std;
bool AdHocLexer::compare(const char* str1, const char* str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

bool AdHocLexer::findKeywords(const char* kw) {
    const char* KEYWORDS[] = {"int",   "float",  "bool",     "double",
                              "main",  "return", "continue", "true",
                              "false", "class",  "include"};
    for (int i = 0; i < sizeof(KEYWORDS) / sizeof(KEYWORDS[0]); i++) {
        if (compare(KEYWORDS[i], kw)) return true;
    }
    return false;
}

void AdHocLexer::lexer(const char* fileName) {
    ifstream Fin(fileName);
    if (!Fin) {
        cout << "Unable to open the file...\n";
        return;
    }

    char ch;
    char buff[1024];

    while (Fin.get(ch)) {
        if (isspace(ch)) continue;

        if (isalpha(ch)) {
            int i = 0;
            buff[i++] = ch;
            while (Fin.get(ch) && (isalnum(ch))) {
                buff[i++] = ch;
            }
            buff[i] = '\0';
            Fin.putback(ch);
            if (findKeywords(buff))
                cout << "KW: " << buff << endl;
            else
                cout << "<ID, " << buff << ">" << endl;
        } else if (isdigit(ch)) {
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
        } else if (ch == '+' || ch == '-' || ch == '/' || ch == '%' ||
                   ch == '*') {
            if (ch == '+')
                cout << "<PLUS, +>" << endl;
            else if (ch == '-')
                cout << "<MINUS, ->" << endl;
            else if (ch == '*')
                cout << "<MULT, *>" << endl;
            else if (ch == '/')
                cout << "<DIV, />" << endl;
            else if (ch == '%')
                cout << "<MOD, %>" << endl;
        } else if (ch == '(' || ch == ')' || ch == '[' || ch == ']' ||
                   ch == '{' || ch == '}') {
            if (ch == '(')
                cout << "<LPARN, (>" << endl;
            else if (ch == ')')
                cout << "<RPARN, )>" << endl;
            else
                cout << "<DELIM, " << ch << ">" << endl;
        } else if (ch == '<' || ch == '>' || ch == '=' || ch == '!' ||
                   ch == '&' || ch == '|') {
            char next;
            Fin.get(next);
            if (ch == '=' && next == '=')
                cout << "<RELOP, ==>" << endl;
            else if (ch == '!' && next == '=')
                cout << "<RELOP, !=>" << endl;
            else if (ch == '<' && next == '=')
                cout << "<RELOP, <=>" << endl;
            else if (ch == '>' && next == '=')
                cout << "<RELOP, >=>" << endl;
            else if (ch == '&' && next == '&')
                cout << "<LOGICOP, &&>" << endl;
            else if (ch == '|' && next == '|')
                cout << "<LOGICOP, ||>" << endl;
            else {
                Fin.putback(next);
                if (ch == '=')
                    cout << "<ASSIGN, =>" << endl;
                else if (ch == '!')
                    cout << "<NOT, !>" << endl;
                else
                    cout << "<RELOP, " << ch << ">" << endl;
            }
        }
    }
}