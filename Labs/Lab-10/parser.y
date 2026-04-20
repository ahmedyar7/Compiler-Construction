%{
#include <stdio.h>
#include <stdlib.h>

/* External declarations linking Lex to Bison */
extern int yylex();
extern int yylineno;
extern char* yytext;

/* Error handling function prototype */
void yyerror(const char *s);
%}

/* Tokens received from the scanner */
%token INT FLOAT BOOL VOID
%token IF ELSE FOR
%token IDENTIFIER NUMBER
%token AND OR NOT EQ NEQ LE GE INC DEC

/* Resolve Dangling Else Shift/Reduce Conflict */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

/* Operator Precedence Definitions */
%left OR
%left AND
%left EQ NEQ
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/'
%right NOT

%%

/* Grammar Rules */
program:
    statements
    ;

statements:
    statements statement
    | statement
    ;

statement:
    type IDENTIFIER ';'
    | type IDENTIFIER '=' expression ';'
    | IDENTIFIER '=' expression ';'
    | IDENTIFIER INC ';'
    | IDENTIFIER DEC ';'
    | IF '(' expression ')' '{' statements '}' %prec LOWER_THAN_ELSE
    | IF '(' expression ')' '{' statements '}' ELSE '{' statements '}'
    | FOR '(' for_init ';' for_cond ';' for_update ')' '{' statements '}'
    | expression ';'
    ;

type:
    INT | FLOAT | BOOL | VOID
    ;

/* For-loop components allowed to be empty or hold specific statements */
for_init:
    type IDENTIFIER '=' expression
    | IDENTIFIER '=' expression
    | /* empty */
    ;

for_cond:
    expression
    | /* empty */
    ;

for_update:
    IDENTIFIER '=' expression
    | IDENTIFIER INC
    | IDENTIFIER DEC
    | /* empty */
    ;

/* Math and Logical Expressions */
expression:
    expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression AND expression
    | expression OR expression
    | NOT expression
    | expression EQ expression
    | expression NEQ expression
    | expression '<' expression
    | expression '>' expression
    | expression LE expression
    | expression GE expression
    | '(' expression ')'
    | IDENTIFIER
    | NUMBER
    ;

%%

/* Custom Error Function that prints the line number */
void yyerror(const char *s) {
    printf("Syntax Error on line %d: %s near '%s'\n", yylineno, s, yytext);
}

/* Main execution block avoiding standard C++ libraries */
int main(void) {
    printf("C++-Lite Validator starting...\n");
    printf("Paste your code to test (Press Ctrl+D on Linux/Mac or Ctrl+Z on Windows to execute):\n");
    
    if (yyparse() == 0) {
        printf("\n=> Validation Successful! No syntax errors found.\n");
    } else {
        printf("\n=> Validation Failed! Please fix the CFG violations.\n");
    }
    return 0;
}