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
"//".                                                                                {cout << "COMMENTS\t\t|\t"           << yytext << endl;}
"int"|"float"|"double"|"bool"|"main"|"void"|"true"|"false"|"continue"|"return"       {cout << "KW\t\t|\t"                 << yytext << endl;}
{DIGIT}+                                                                             {cout << "DIGIT\t\t|\t"              << yytext << endl;}
{ID}                                                                                 {cout << "ID\t\t|\t"                 << yytext << endl;}
{STRING}                                                                             {cout << "STRINGS_LITERALS\t\t|\t"   << yytext << endl;}
"+"|"-"|"/"|"*"|"%"                                                                  {cout << "ARITHEMATIC-OP\t|\t"     << yytext << endl;}
"("|")"|"["|"]"|"{"|"}"|";"|"."|","                                                  {cout << "DELIMITER\t|\t"          << yytext << endl;}
"&&"|"||"|"!"|"=="|"!="                                                              {cout << "CONDITION\t|\t"          << yytext << endl;}
"="                                                                                  {cout << "ASSIGN\t\t|\t"             << yytext << endl;}
"<"|"<="|">"|">="                                                                    {cout << "RELATION-OP\t|\t"             << yytext << endl;}
[ \t\n]                                                                              {/*IGNORE WHITESPACES*/}
.                                                                                    {cout << "UNKNOWN\t\t|\t"            << yytext << endl;} 
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