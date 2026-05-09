%{
#include <iostream>
#include <string>
#include <vector>

// External functions from Flex
extern int yylex();
extern int yylineno;
extern char* yytext;

void yyerror(const char *s);
%}

/* Token Definitions - Must match Lexer return values */
%token HEADER NAMESPACE TYPE STREAM_OP UPDATE_OP 
%token IOSTREAM RELATION_OP BUILTINS IF WHILE RETURN 
%token ID DIGIT

/* Precedence Rules to handle math expressions correctly */
%left '+' '-'
%left '*' '/' '%'

%%

/* The Start Symbol */
program:
    header_section namespace_section declarations functions {
        std::cout << "\n--- [Parsing Complete] Syntax is Valid ---" << std::endl;
    }
    ;

header_section:
    header_section HEADER
    | HEADER
    ;

namespace_section:
    NAMESPACE ';' 
    | /* empty */
    ;

declarations:
    declarations decl
    | /* empty */
    ;

decl:
    TYPE ID '(' TYPE ')' ';' 
    | TYPE ID ';'
    ;

functions:
    functions function
    | function
    ;

function:
    TYPE ID '(' params ')' '{' body '}'
    ;

params:
    TYPE ID
    | TYPE
    | params ',' TYPE ID
    | /* empty */
    ;

body:
    body statement
    | /* empty */
    ;

statement:
    TYPE ID ';'
    | TYPE ID '=' expr ';'
    | ID '=' expr ';'
    | ID UPDATE_OP expr ';'
    | ID UPDATE_OP ';'
    | WHILE '(' condition ')' '{' body '}'
    | IF '(' condition ')' '{' body '}'
    | IOSTREAM STREAM_OP expr ';'
    | BUILTINS '(' args ')' ';'
    | RETURN expr ';'
    ;

expr:
    ID
    | DIGIT
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '%' expr
    | '(' expr ')'
    | BUILTINS '(' args ')'
    ;

args:
    arg_list
    | /* empty */
    ;

arg_list:
    arg_list ',' arg
    | arg
    ;

arg:
    ID
    | DIGIT
    | '&' ID
    ;

condition:
    expr RELATION_OP expr
    ;

%%

/* Error handling */
void yyerror(const char *s) {
    std::cerr << "[Syntax Error] Line " << yylineno << ": " << s << " at token: " << yytext << std::endl;
}

int main() {
    std::cout << "Starting C++-Lite Parser..." << std::endl;
    if (yyparse() == 0) {
        std::cout << "Process finished successfully." << std::endl;
    } else {
        std::cout << "Parsing failed." << std::endl;
    }
    return 0;
}