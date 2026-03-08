%{
#include <iostream>
#include <stdio.h>

using namespace std;
%}

/*Regular Expression*/
DIGITS       [0-9]
ID           [a-zA-Z_][a-zA-Z0-9_]*
STRINGS      \"[^\"]*\"

%% 
"//".*                                                         {cout << "COMMENT: " << yytext << endl;}
"int"|"float"|"bool"|"main"|"return"|"continue"|"true"|"false" {cout << "KEYWORD: " << yytext << endl;} 
"["|"]"|"("|")"|"{"|"}"|";"|","|":"                            {cout << "DELIMITER: " << yytext << endl;}
{DIGITS}+                                                      {cout << "DIGITS: " << yytext << endl;}
{ID}                                                           {cout << "ID: " << yytext << endl;}
{STRINGS}                                                      {cout << "STRINGS_LITERALS: " << yytext << endl;}
"="|"/"|"-"|"+"|"*"                                            {cout << "ARITHEMATIC-OP: " << yytext << endl;}
"<"|">"|"<="|">="                                              {cout << "RELATION-OPS: " << yytext << endl;}
"&&"|"||"|"!"                                                  {cout << "CONDITION-OP: " << yytext << endl;}
[ \t\n]+                                                       {/*SKIP WHITESPACES*/}
%%

int yywrap(){
    return -1;
}

int main(){

    FILE *file = fopen("input.cpp","r");
    if(!file){
        cout << "Unable to open the file" << endl;
        return -1;
    }

    yyin = file;

    cout << "\n==== START TOKENIZATION ====\n\n";
    yylex();
    cout << "\n==== END TOKENIZAITON ====";

    fclose(file);

    return 0;
}