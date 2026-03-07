%{
#include <iostream>
#include <stdio.h>

using namespace std;
%}

/* Defination regarding the Regular Expression*/

DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*
STRINGS  \"[^\"]*\"

/* RULES                                                       ACTIONS                         */
%%
"//".*                                                  {cout << "COMMENT: " << yytext << endl;}
"int"|"float"|"bool"|"return"|"continue"|"true"|"false" {cout << "KEYWORD: " << yytext << endl;}
{DIGIT}+                                                {cout << "DIGIT: " << yytext << endl;}
{ID}                                                    {cout << "ID: " << yytext << endl;}
{STRINGS}                                               {cout << "STRING_LITERAL: " << yytext << endl;}
"="|"+"|"-"|"/"|"*"|"%"                                 {cout << "OP: " << yytext << endl;}
"("|")"|"["|"]"|"{"|"}"                                 {cout << "DELIMTER: " << yytext << endl;}
[\t\n]+                                                 {/*Skip whitespaces*/}
.                                                       {cout << "UNKNOWN " << yytext << endl;}
%%

int yywrap() {return 1;}

int main(){

    FILE *file = fopen("input.cpp","r");
    if(!file){
        cout << "ERROR: Cannot open the input.cpp!" << endl;
        return -1;
    }

    yyin = file;

    cout << "--- START TOKENIZATION ---" << endl;
    yylex();
    cout << "--- TOKENIZATION COMPLETE ---" << endl;

    fclose(file);
    
    return 0;
}