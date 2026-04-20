%{
#include <stdio.h>
#include <stdlib.h>

/* Define the type of data passed from Lex to Bison here inside the C block */
#define YYSTYPE int

/* External declarations linking Lex to Bison */
extern int yylex();
extern int yylineno;
extern char* yytext;

/* Error handling function prototype */
void yyerror(const char *s);
%}

/* Tokens received from the scanner */
%token NUM EOL

%%

/* Handle multiple lines of input continuously */
program:
    | program line
    ;

line:
    EOL
    | E EOL { printf("\n"); } /* Print newline after the postfix expression is complete */
    ;

/* Expression Rule (Addition and Subtraction) */
E:
      E '+' T { printf("+ "); }
    | E '-' T { printf("- "); }
    | T
    ;

/* Term Rule (Multiplication and Division - Higher Precedence) */
T:
      T '*' F { printf("* "); }
    | T '/' F { printf("/ "); }
    | F
    ;

/* Factor Rule (Parentheses and Numbers - Highest Precedence) */
F:
      '(' E ')' 
    | NUM { printf("%d ", $1); } /* Print the number as soon as it is reduced */
    ;

%%

/* Custom Error Function */
void yyerror(const char *s) {
    printf("Syntax Error on line %d: %s\n", yylineno, s);
}

/* Main execution block avoiding standard C++ libraries */
int main(void) {
    printf("Infix to Postfix Converter\n");
    printf("Enter standard mathematical expressions (e.g., 3 + 4 * 5):\n");
    printf("Press Ctrl+D (Linux/Mac) or Ctrl+Z (Windows) to exit.\n\n");
    
    yyparse();
    return 0;
}