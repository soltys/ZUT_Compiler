%{
#include <cstdio>
#include <iostream>
#include "Scanner.hpp"
using namespace std;


#include "parser.hh"

typedef PSLang::Parser::token token;

%}

%option debug
%option nodefault
%option yyclass="Scanner"
%option noyywrap
%option c++

%%

[ \t] ;
[\n]  ;

[0-9]+\.[0-9]+ {yylval->fval = atof(yytext); return token::FLOAT_NUMBER;}
[0-9]+ {yylval->ival = atoi(yytext); return token::INT_NUMBER;}
[a-zA-Z0-9]+ { yylval->sval = strdup(yytext); return token::STRING;}
.    ;

%%
