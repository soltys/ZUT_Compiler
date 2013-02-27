%skeleton "lalr1.cc"
%require "2.5"
%debug
%defines "parser.hh"
%output  "parser.cc"
%define namespace "PSLang"
%define parser_class_name "Parser"

%code requires {
    namespace PSLang{
        class Driver;
        class Scanner;
    }


}

%lex-param {Scanner &scanner}
%parse-param {Scanner &scanner}

%lex-param {Driver &driver}
%parse-param {Driver &driver}

%code{
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "Driver.h"
 
static int yylex(PSLang::Parser::semantic_type *yylval,
                     PSLang::Scanner  &scanner,
                    PSLang::Driver   &driver);
}


// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
%union {
	int ival;
	float fval;
	char *sval;
}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT_NUMBER
%token <fval> FLOAT_NUMBER
%token <sval> STRING

%%
// this is the actual grammar that bison will parse, but for right now it's just
// something silly to echo to the screen what bison gets from flex.  We'll
// make a real one shortly:
snazzle:
	snazzle INT_NUMBER       { std::cout << "bison found an int: " << $2 << std::endl; }
	| snazzle FLOAT_NUMBER   { std::cout << "bison found a float: " << $2 << std::endl; }
	| snazzle STRING  { std::cout << "bison found a string: " << $2 << std::endl; }
	| INT_NUMBER            { std::cout << "bison found an int: " << $1 << std::endl; }
	| FLOAT_NUMBER          { std::cout << "bison found a float: " << $1 << std::endl; }
	| STRING         { std::cout << "bison found a string: " << $1 << std::endl; }
	;
%%

void PSLang::Parser::error( const PSLang::Parser::location_type &l,
                            const std::string &err_message)
{
    std::cerr << "Error: "<< err_message << "\n";
}

#include "Scanner.hpp"
static int yylex(PSLang::Parser::semantic_type *yylval,
                 PSLang::Scanner  &scanner,
                 PSLang::Driver   &driver)
{
   return( scanner.yylex(yylval) );
}