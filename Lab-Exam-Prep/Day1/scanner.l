%{
#include <iostream>
#include <string>
#include "parser.tab.c" // Ensure this matches your Bison output filename
%}

%option noyywrap
%option yylineno

%%
"#include"[ ]*<[^>]+>           { return HEADER; }
"using namespace std;"          { return NAMESPACE; }
"int"|"long"|"void"             { return TYPE; }
"cout"|"cin"                    { return IOSTREAM; }
"<<"|">>"                       { return STREAM_OP; }
"++"|"--"                       { return UPDATE_OP; }
"+="|"-="|"*="|"/="|"%="        { return UPDATE_OP; }
"=="|"!="|">"|"<"|">="|"<="     { return REL_OP; }
"printf"|"scanf"|"pow"          { return BUILTIN; }
"if"                            { return IF; }
"while"                         { return WHILE; }
"return"                        { return RETURN; }

[a-zA-Z_][a-zA-Z0-9_]*          { return ID; }
[0-9]+                          { return NUM; }

[ \t\n]+                        { /* ignore whitespace */ }

"{"|"}精彩"|"("|")"|";"|","|"="|"&"|"+"|"*"|"%"|"/"  { return yytext[0]; }

.                               { std::cerr << "Unknown character: " << yytext[0] << " on line " << yylineno << std::endl; }
%%