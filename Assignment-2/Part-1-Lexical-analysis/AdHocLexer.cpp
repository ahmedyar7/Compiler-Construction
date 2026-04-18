#include "AdHocLexer.h"

#include <cctype>
#include <cstring>

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
    std::ifstream Fin(fileName);
    if (!Fin) {
        std::cout << "Unable to open the file...\n";
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
                std::cout << "KW: " << buff << std::endl;
            else
                std::cout << "<ID, " << buff << ">" << std::endl;
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
            std::cout << "<NUM, " << buff << ">" << std::endl;
        } else if (ch == '+' || ch == '-' || ch == '/' || ch == '%' ||
                   ch == '*') {
            if (ch == '+')
                std::cout << "<PLUS, +>" << std::endl;
            else if (ch == '-')
                std::cout << "<MINUS, ->" << std::endl;
            else if (ch == '*')
                std::cout << "<MULT, *>" << std::endl;
            else if (ch == '/')
                std::cout << "<DIV, />" << std::endl;
            else if (ch == '%')
                std::cout << "<MOD, %>" << std::endl;
        } else if (ch == '(' || ch == ')' || ch == '[' || ch == ']' ||
                   ch == '{' || ch == '}') {
            if (ch == '(')
                std::cout << "<LPARN, (>" << std::endl;
            else if (ch == ')')
                std::cout << "<RPARN, )>" << std::endl;
            else
                std::cout << "<DELIM, " << ch << ">" << std::endl;
        } else if (ch == '<' || ch == '>' || ch == '=' || ch == '!' ||
                   ch == '&' || ch == '|') {
            char next;
            Fin.get(next);
            if (ch == '=' && next == '=')
                std::cout << "<RELOP, ==>" << std::endl;
            else if (ch == '!' && next == '=')
                std::cout << "<RELOP, !=>" << std::endl;
            else if (ch == '<' && next == '=')
                std::cout << "<RELOP, <=>" << std::endl;
            else if (ch == '>' && next == '=')
                std::cout << "<RELOP, >=>" << std::endl;
            else if (ch == '&' && next == '&')
                std::cout << "<LOGICOP, &&>" << std::endl;
            else if (ch == '|' && next == '|')
                std::cout << "<LOGICOP, ||>" << std::endl;
            else {
                Fin.putback(next);
                if (ch == '=')
                    std::cout << "<ASSIGN, =>" << std::endl;
                else if (ch == '!')
                    std::cout << "<NOT, !>" << std::endl;
                else
                    std::cout << "<RELOP, " << ch << ">" << std::endl;
            }
        }
    }
}