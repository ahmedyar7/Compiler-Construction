%{
#include <iostream>
#include <stdio.h>

using namespace std;
%}

DIGIT               [0-9]
IDENTIFIER          [a-zA-Z_][a-zA-Z0-9_]*
STRING              \"[^\"]*\"

%%
"//".                                                                                     {cout << "COMMENT:" << endl;}
"int"|"float"|"double"|"bool"|"true"|"false"|"return"|"continue"|"if"|"else"|"else if"    {cout << "ID: " << yytext << endl;}
{DIGIT}+                                                                                  {cout << "DIGIT: " << yytext << endl;}
{IDENTIFIER}                                                                              {cout << "ID: " << yytext << endl;}
{STRING}                                                                                  {cout << "STRINGS_LITERALS: " << yytext << endl;}
"+"|"-"|"/"|"*"|"%"                                                                       {cout << "ARITHEMATIC-OPS: " << yytext << endl;}
"<"|"<="|">"|">="                                                                         {cout << "RELATION-OP: " << yytext << endl;}
"&&"|"||"|"!"                                                                             {cout << "CONDITION-OP: " << yytext << endl;}
";"|":"|","|"("|")"|"["|"]"|"{"|"}"                                                       {cout << "DELIMITER: " << yytext << endl;}
[ \t\n]                                                                                   {/*WHITESPACES*/}
.                                                                                         {cout << "UNKNOWN:" << endl;}
%%

int yywrap(){
    return -1;
}

int main(){

    FILE* file = fopen("input.cpp","r");
    if(!file){
        cout << "Error: Unable to open the file" << endl;
        return -1;
    }

    yyin = file;

    cout << "\n==== START TOKENIZATION  ====\n";
    yylex();
    cout << "\n==== END TOKENIZAITON  ====\n";

    fclose(file);

    return 0;
}