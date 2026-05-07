%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex();

/* Basic Symbol Table function placeholder */
void add_to_symbol_table(char* id, int rows, int cols, int width) {
    printf("/* Declared %s: %d rows, %d columns */\n", id, rows, cols);
}
%}

%union { 
    int num; 
    char* id; 
}

/* Token Definitions */
%token <id> ID
%token <num> NUM
%token INT           /* Updated from TYPE to INT */

/* Non-terminal Types */
%type <num> expr
%type <num> statement
%type <num> assignment
%type <num> declaration

%%

/* Grammar Rules */
program: statements;

statements: statement 
          | statements statement
          ;

statement: declaration 
         | assignment
         ;

/* Array Declaration Rule */
declaration: INT ID '[' NUM ']' '[' NUM ']' ';' { 
    add_to_symbol_table($2, $4, $7, 4); 
};

/* 3AC Generation for Array Access */
assignment: ID '=' ID '[' expr ']' '[' expr ']' ';' {
    /* Note: For a fully complete 3AC, you would look up the columns of $3 
       from the symbol table here instead of hardcoding '4'. 
    */
    printf("t1 = %d * 4\n", $5);       // Multiply row index by total columns
    printf("t2 = t1 + %d\n", $8);      // Add column index
    printf("t3 = t2 * 4\n");           // Multiply by data width (int = 4 bytes)
    printf("%s = %s[t3]\n", $1, $3);   // Final assignment
};

expr: NUM { $$ = $1; };

%%

/* Error handling function */
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

/* Main execution block */
int main() {
    // Starts the parsing process
    return yyparse();
}