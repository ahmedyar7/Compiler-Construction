%{
#include "parser.tab.h"
%}

%option noyywrap

%%
"int"       { return INT; }
[0-9]+      { yylval.num = atoi(yytext); return NUM; }
[a-zA-Z]+   { yylval.id = strdup(yytext); return ID; }
"["         { return '['; }
"]"         { return ']'; }
";"         { return ';'; }
"="         { return '='; }
[ \t\n]+    ;
.           ;
%%