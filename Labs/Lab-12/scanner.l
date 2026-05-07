%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
%}
%option noyywrap
%%
[ \t\n\r]+      {}
"int"|"void"    { return TYPE; }
"return"        { return RETURN; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return ID; }
[0-9]+          { yylval.str = strdup(yytext); return NUM; }
"="             { return ASSIGN; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MUL; }
"/"             { return DIV; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
","             { return COMMA; }
";"             { return SEMI; }
.               { return yytext[0]; }
%%