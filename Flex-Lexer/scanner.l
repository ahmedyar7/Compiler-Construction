%{
#include <iostream>
#include <stdio.h>

using namespace std;
%}

/*Defining the Regular Expression*/

DIGIT              [0-9]
ID                 [a-zA-Z_][a-zA-Z0-9_]*
STRING             \"[^\"]*\"

/*Defining the RULES and ACTIONS*/

%%
"//".                                                                                {cout << "COMMENTS -> "           << yytext << endl;}
"int"|"float"|"double"|"bool"|"main"|"void"|"true"|"false"|"continue"|"return"       {cout << "KW -> "                 << yytext << endl;}
{DIGIT}+                                                                             {cout << "DIGIT -> "              << yytext << endl;}
{ID}                                                                                 {cout << "ID ->  "                 << yytext << endl;}
{STRING}                                                                             {cout << "STRINGS_LITERALS -> "   << yytext << endl;}
"+"|"-"|"/"|"*"|"%"                                                                  {cout << "ARITHEMATIC-OP -> "     << yytext << endl;}
"("|")"|"["|"]"|"{"|"}"|";"|"."|","                                                  {cout << "DELIMITER -> "          << yytext << endl;}
"&&"|"||"|"!"|"=="|"!="                                                              {cout << "CONDITION -> "          << yytext << endl;}
"="                                                                                  {cout << "ASSIGN -> "             << yytext << endl;}
[ \t\n]                                                                              {/*IGNORE WHITESPACES*/}
.                                                                                    {cout << "UNKNOWN -> "            << yytext << endl;} 
%%

int yywrap(){
    return -1;
}

int main(){

    FILE *file = fopen("input.cpp","r");
    if(!file){
        cout << "\nUnable to open the file (input.cpp)\n";
        return -1;
    }

    yyin = file;

    cout << "\n--- START TOKENIZATION ---\n";
    yylex();
    cout << "\n--- END TOKENIZATION ---\n";

    fclose(file);

    return 0;
}