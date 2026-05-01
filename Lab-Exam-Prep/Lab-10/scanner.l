%{
#include "parser.tab.hpp"
#include <stdio.h>
#include <stdlib.h>
%}

%option yylineno;
%option noyywarp;

%
"int"      {return INT;}
"float"    {return FLOAT;}
"bool"     {return BOOL;}
"void"     {return VOID;}

"if"       {return IF;}
"else"     {return ELSE;}
"for"      {return FOR;}
"while"    {return WHILE;}

"||"       {return OR;}
"&&"       {return AND;}
"!"        {return NOT;}

"=="       {return EQ;}
"!="       {return NEQ;}
"<"        {return LT;}
"<="       {return LEQ;}
">"        {return GT;}
">="       {return GEQ;}

"++"       {return INC;}
"--"       {return DEC;}
"+"        {return PLUS;}
"-"        {return MINUS;}
"*"        {return MUL;}
"/"        {return DIV;}
"="        {return ASSIGN;}

";"        {return SEMI;}
"("        {return LPAREN;}
")"        {return RPAREN;}
"{"        {return LBRACE;}
"}"        {return RBRACE;}

[a-zA-Z_][a-zA-Z0_9]* {return ID;}
[0-9]+                {return NUM}
[ \t\r\n]+            {/*ignore whitespaces that are present.*/}
%