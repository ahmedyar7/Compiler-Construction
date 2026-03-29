%{
#include <iostream>
#include <stdio.h>

using namespace std;
%}

/*Regular expression*/

DIGIT [0-9]
ID [a-zA-Z_][a-zA-Z0-9_]*
STRING \"[^\"]*\"

%%
"//." {cout << "COMMENTS: " << yytext << endl;}
"int"|"main"|"bool"|"true"|"false"|"void"|"double" {cout << "KW: " << yytext << endl;}
{ID} {cout << "ID: " << yytext << endl;}
{DIGIT}+ {cout << "DIGIT: " << yytext << endl;}
{STRING} {cout << "STRINGS_LITERALS: " << yytext << endl;}
"&&"|"||"|"=="|"!=" {cout << "CONDITION_OP: " << yytext << endl;}
"<"|"<="|">"|">=" {cout << "RELATION-OP: " << yytext << endl;}
"=" {cout << "ASSIGN: " << yytext << endl;}
"+"|"++"|"--"|"-"|"/"|"%"|"*" {cout << "ARITHEMTATIC_OP: " << yytext << endl;}
"("|")"|"["|"]"|"{"|"}" {cout << "DELIMTERS: " << yytext << endl;}
[ \t\n.] {cout << "WHITESPACES: " << yytext << endl;}
. {cout << "UNKNOWN " << yytext << endl;}
%%

int yywrap(){
    return -1;
}

int main(){

    FILE *file = fopen("input.cpp","r");
    if(!file){
        cout << "Unable to open the file\n";
        return -1;
    }
    yyin = file;
    cout << "--- Start Tokenization ---\n";
    yylex();
    cout << "--- End Tokenization ---\n" << endl;
 
    return 0;
}
