%{
#include <iostream>
#include <string>
#include "parser.tab.hpp" // Contains token definitions from Bison
%}

%option noyywrap
%option yylineno

%%

"#include"[ ]*<[^>]+>          { return HEADER; }
"using namespace std;"         { return NAMESPACE; }

"int"|"long"|"float"|"double"  { return TYPE; }
"return"                       { return RETURN; }
"while"                        { return WHILE; }
"if"                           { return IF; }

"printf"|"scanf"|"pow"         { return BUILTIN; }
"cout"|"cin"                   { return IOSTREAM; }
"<<"|">>"                      { return STREAM_OP; }

[a-zA-Z_][a-zA-Z0-9_]* { return ID; }
[0-9]+                         { return NUM; }

"++"|"--"|"+="|"-="|"/="|"%="  { return UPDATE_OP; }
"=="|"!="|"<="|">="|"<"|">"    { return REL_OP; }

[ \t\n\r]                      { ; /* Skip whitespace */ }
.                              { return yytext[0]; }

%%