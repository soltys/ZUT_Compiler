%{
#include <cstdio>
#include <iostream>
#include "Scanner.hpp"
using namespace std;


#include "parser.hh"

typedef PSLang::Parser::token token;
#define yyterminate() return(0)
%}

%option debug
%option nodefault
%option yyclass="Scanner"
%option noyywrap
%option c++


STARTBLOCK  "\{"
ENDBLOCK    "\}"
LPAREN      "\("
RPAREN      "\)"
PLUS        "\+"
MINUS       "\-"
MULTIPLY    "\*"
DIVISION    "\/"
ASSIGNMENT  "\="
%%

[ \t] ;
[\n]   {incrementLineNumber();};

[0-9]+\.[0-9]+ {yylval->fval = atof(yytext); return token::FLOAT_NUMBER;}
[0-9]+ {yylval->ival = atoi(yytext); return token::INT_NUMBER;}
\"[^"]*\" { yylval->sval = strdup(yytext); return token::STRING;}
[a-zA-Z0-9_-]+ { yylval->sval = strdup(yytext); return token::IDENTIFIER;}

{ASSIGNMENT} {return token::OP_ASSIGNMENT;}

{STARTBLOCK} {return token::STARTBLOCK;}
{ENDBLOCK}   {return token::ENDBLOCK;}
{LPAREN} {return token::LPAREN;}
{RPAREN} {return token::RPAREN;}

<<EOF>> {
			yyterminate();
		}
.    ;

%%
/*
{PLUS} {return token::OP_PLUS;}
{MINUS} {return token::OP_MINUS;}
{MULTIPLY} {return token::OP_MULTIPLY;}
{DIVISION} {return token::OP_DIVISION;}
*/