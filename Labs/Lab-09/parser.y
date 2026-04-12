%{
#include <iostream>
#include <cstdlib>

extern int yylex();
extern int yylineno;
void yyerror(const char *s);
%}

/* Token Definitions */
%token HEADER NAMESPACE TYPE RETURN WHILE IF BUILTIN IOSTREAM STREAM_OP ID NUM UPDATE_OP REL_OP

/* Precedence to handle math expressions */
%left '+' '-'
%left '*' '/' '%'

%%

program: 
    headers opt_namespace declarations functions { 
        std::cout << "\n[Success] Program is Syntactically Correct C++-Lite!" << std::endl; 
    };

headers: HEADER headers | HEADER ;

opt_namespace: NAMESPACE | /* empty */ ;

declarations: decl declarations | /* empty */ ;
decl: TYPE ID '(' TYPE ')' ';' ;

functions: function functions | function ;

function: TYPE ID '(' params ')' '{' body '}' ;

params: TYPE ID | TYPE | /* empty */ ;

body: statement body | statement | /* empty */ ;

statement: 
    TYPE ID ';' 
    | TYPE ID '=' expr ';'
    | ID '=' expr ';'
    | ID UPDATE_OP ';'
    | WHILE '(' condition ')' '{' body '}'
    | BUILTIN '(' args ')' ';'
    | IOSTREAM STREAM_OP expr ';'
    | RETURN expr ';' ;

expr: 
    ID 
    | NUM 
    | BUILTIN '(' args ')' 
    | expr '+' expr 
    | expr '*' expr 
    | expr '%' expr ;

args: arg_list | /* empty */ ;
arg_list: arg ',' arg_list | arg ;
arg: ID | NUM | '&' ID | '"' ; 

condition: 
    expr REL_OP expr 
    | ID REL_OP NUM ;

%%

void yyerror(const char *s) {
    std::cerr << "[Error] Line " << yylineno << ": " << s << std::endl;
}

int main() {
    std::cout << "--- C++-Lite Syntax Validator Initialized ---" << std::endl;
    return yyparse();
}