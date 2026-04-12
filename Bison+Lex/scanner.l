%{
#include <iostream>
#include <string>
#include "parser.tab.hpp"
%}

%option noyywrap
%option yylineno

%%
"#include"[ ]*<[^>]+>    { return HEADER; }
"using namespace std;"   { return NAMESPACE; }

"int"|"float"|"bool"|"void"|"double" { return TYPE; }

"return"    { return RETURN; }
"while"     { return WHILE; }
"if"        { return IF; }

[0-9]+                   { return NUM; }
[a-zA-Z_][a-zA-Z0-9_]* { return ID; }

"++"|"--"|"+="|"-="|"/="|"%=" { return UPDATE_OP; }
"=="|"!="|"<="|">="|"<"|">"    { return REL_OP; }

"<<"|">>"                { return STREAM_OP; }
"cin"|"cout"              { return IOSTREAM; }
"printf"|"scanf"|"pow"    { return BUILTIN; }

[ \t\n\r]                { ; /* SKIPS WHITESPACES */ }
.                        { return yytext[0]; }  
%%