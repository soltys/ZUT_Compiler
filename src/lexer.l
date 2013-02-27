%{
#include <cstdio>
#include <iostream>

using namespace std;
#define YY_DECL extern "C" int yylex()

#include "parser.hh"

%}

%%

[ \t] ;
[\n]  ;

[0-9]+\.[0-9]+ {yylval.fval = atof(yytext); return FLOAT_NUMBER;}
[0-9]+ {yylval.ival = atoi(yytext); return INT_NUMBER;}
[a-zA-Z0-9]+ { yylval.sval = strdup(yytext); return STRING;}
.    ;

%%
