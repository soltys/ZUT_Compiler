%{
#include <cstdio>
#include <iostream>
#include "Scanner.hpp"


#include "parser.hh"

typedef PSLang::Parser::token token;
#define yyterminate() return(0)

#define SAVE_TOKEN yylval->string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval->token = token::t)

%}

%option debug
%option nodefault
%option yyclass="Scanner"
%option noyywrap
%option c++
%option yylineno


%%

[ \t]                   ;

[\n]					{yylineno++;}

"for"					return TOKEN(TFOR);
"if"					return TOKEN(TIF);
"else"					return TOKEN(TELSE);
"while"					return TOKEN(TWHILE);
"to"					return TOKEN(TTO);

[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return token::TIDENTIFIER;
[0-9]+\.[0-9]*          SAVE_TOKEN; return token::TDOUBLE;
[0-9]+                  SAVE_TOKEN; return token::TINTEGER;
"="                     return TOKEN(TEQUAL);
"=="                    return TOKEN(TCEQ);
"!="                    return TOKEN(TCNE);
"<"                     return TOKEN(TCLT);
"<="                    return TOKEN(TCLE);
">"                     return TOKEN(TCGT);
">="                    return TOKEN(TCGE);
"&&"					return TOKEN(TAND);
"||"					return TOKEN(TOR);
"("                     return TOKEN(TLPAREN);
")"                     return TOKEN(TRPAREN);
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
"["						return TOKEN(TLBRACKET);
"]"						return TOKEN(TRBRACKET);
"."                     return TOKEN(TDOT);
","                     return TOKEN(TCOMMA);
"*"                     return TOKEN(TMUL);
"/"                     return TOKEN(TDIV);
"+"                     return TOKEN(TPLUS);
"-"                     return TOKEN(TMINUS);
";"					return TOKEN(TSEMICOLON);

<<EOF>> {
			yyterminate();
		}
.    ;

%%

//
