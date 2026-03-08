%{
#include <iostream>
#include <cstdio>
using namespace std;

int token_count = 0;
%}

/* Definitions: Helper Regex */
DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*
STRING   \"[^\"]*\"

%%
"//".* { cout << "COMMENT: " << yytext << endl; }
"int"|"double"|"return" { cout << "KEYWORD: " << yytext << endl; }
{STRING}                { cout << "STRING_LITERAL: " << yytext << endl; }
{ID}                    { cout << "IDENTIFIER: " << yytext << endl; }
{DIGIT}+                { cout << "NUMBER: " << yytext << endl; }
"="|"+"|"*"|"&"         { cout << "OPERATOR: " << yytext << endl; }
"("|")"|"{"|"}"|","|";" { cout << "PUNCTUATION: " << yytext << endl; }
[ \t\n]+                { /* Skip whitespace */ }
.                       { cout << "UNKNOWN: " << yytext << endl; }
%%

int yywrap() { return 1; }

int main() {
    // Open the input file
    FILE *myfile = fopen("input.cpp", "r");
    if (!myfile) {
        cout << "Error: Cannot open input.cpp!" << endl;
        return -1;
    }
    
    // Set Flex to read from the file instead of stdin
    yyin = myfile;
    
    cout << "--- Starting Tokenization ---" << endl;
    yylex();
    cout << "--- Tokenization Complete ---" << endl;
    
    fclose(myfile);
    return 0;
}