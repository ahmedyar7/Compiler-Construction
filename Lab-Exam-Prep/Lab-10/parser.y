%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yylineno;
void yyerror(const char *s);
%}

%token INT FLOAT BOOL VOID
%token IF ELSE FOR WHILE
%token OR AND NOT
%token EQ NEQ LT LEQ GT GEQ
%token INC DEC PLUS MINUS MUL DIV ASSIGN
%token SEMI LPAREN RPAREN LBRACE RBRACE
%token ID
%token NUM

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%left OR
%left AND
%left EQ NEQ
%left LT GT LEQ GEQ
%left PLUS MINUS
%left MUL DIV
%right NOT

%%
program:
    statements;

statements:
    statement statements
    | statement
    | /*empty*/
    ;

statement:
    declaration
    | assignment
    | if_stmt 
    | for_stmt
    | while_stmt
    | block
    ;

block:
    LBRACE statement RBRACE
    | LBRACE RBRACE
    ;

declaration:
    type ID SEMI
    | type ID ASSIGN expr SEMI
    ;

assignment:
    ID ASSIGN expr SEMI
    | ID INC SEM
    | ID DEC SEM
    ;

type:
    INT | FLOAT | BOOL | VOID
    ;



%%