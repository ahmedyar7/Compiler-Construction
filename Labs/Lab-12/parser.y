%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int yylex();
void yyerror(const char *s);
int temp_count = 1;
char* new_temp();
char arg_queue[20][50];
int queue_head = 0;
%}

%union {
    char* str;
}

%token <str> ID NUM TYPE RETURN
%token ASSIGN PLUS MINUS MUL DIV LPAREN RPAREN LBRACE RBRACE COMMA SEMI

%type <str> expr statement statements function_call arg_list argument

%left PLUS MINUS
%left MUL DIV

%%
program: statements
       ;

statements: statement
          | statements statement
          ;

statement: TYPE ID ASSIGN expr SEMI {
             printf("%s = %s\n", $2, $4);
         }
         | ID ASSIGN expr SEMI {
             printf("%s = %s\n", $1, $3);
         }
         | expr SEMI {}
         ;

expr: expr PLUS expr {
        $$ = new_temp();
        printf("%s = %s + %s\n", $$, $1, $3);
    }
    | expr MINUS expr {
        $$ = new_temp();
        printf("%s = %s - %s\n", $$, $1, $3);
    }
    | ID { $$ = $1; }
    | NUM { $$ = $1; }
    | function_call { $$ = $1; }
    ;

function_call: ID LPAREN arg_list RPAREN {
                 int i;
                 for(i = 0; i < queue_head; i++) {
                     printf("param %s\n", arg_queue[i]);
                 }
                 $$ = new_temp();
                 printf("%s = call %s, %d\n", $$, $1, queue_head);
                 queue_head = 0;
             }
             | ID LPAREN RPAREN {
                 $$ = new_temp();
                 printf("%s = call %s, 0\n", $$, $1);
             }
             ;

arg_list: argument {
            strcpy(arg_queue[queue_head++], $1);
        }
        | arg_list COMMA argument {
            strcpy(arg_queue[queue_head++], $3);
        }
        ;

argument: expr { $$ = $1; }
        ;
%%

char* new_temp() {
    char* t = (char*)malloc(10);
    sprintf(t, "t%d", temp_count++);
    return t;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    yyparse();
    return 0;
}