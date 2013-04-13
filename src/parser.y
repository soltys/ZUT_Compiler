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
        class Node;
        class NBlock;
        class NExpression;
        class NStatement;
        class NIdentifier;
        class NVariableDeclaration;
        class NIfStatement;        
        class NWhileStatement;
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
#include "Node.h"
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
    PSLang::Node *node;
    PSLang::NBlock *block;
    PSLang::NExpression *expr;
    PSLang::NStatement *stmt;
    PSLang::NIdentifier *ident;
    PSLang::NVariableDeclaration *var_decl;
    std::vector<NVariableDeclaration*> *varvec;
    std::vector<NExpression*> *exprvec;
    std::string *string;
    int token;
}


%token <string> TIDENTIFIER TINTEGER TDOUBLE
%token <token> TSEMICOLON TTO
%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL
%token <token> TAND TOR
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT 
%token <token> TPLUS TMINUS TMUL TDIV
%token <token> TFOR TIF TELSE TWHILE 

/* Operator precedence for mathematical operators */
%right TEQUAL
%left TOR
%left TAND
%left TCEQ TCNE TCLT TCLE TCGT TCGE 
%left TPLUS TMINUS
%left TMUL TDIV


%type <ident> ident
%type <expr> numeric expr 
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl if_stmt while_stmt

%start program

%%
// this is the actual grammar that bison will parse, but for right now it's just
// something silly to echo to the screen what bison gets from flex.  We'll
// make a real one shortly:

program : stmts { driver.programBlock = $1; }
        ;
        
stmts : stmt { $$ = new NBlock(); $$->statements.push_back($<stmt>1); }
      | stmts stmt { $1->statements.push_back($<stmt>2); }
      ;

stmt : var_decl  TSEMICOLON
     | func_decl TSEMICOLON       
     | expr TSEMICOLON { $$ = new NExpressionStatement(*$1); }
     | if_stmt
     | while_stmt
     ;

block : TLBRACE stmts TRBRACE { $$ = $2; }
      | TLBRACE TRBRACE { $$ = new NBlock(); }
      ;

var_decl : ident ident { $$ = new PSLang::NVariableDeclaration(*$1, *$2); }
         | ident ident TEQUAL expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
         ;
        
func_decl : ident ident TLPAREN func_decl_args TRPAREN block 
            { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6); delete $4; }
          ;
    
func_decl_args :  { $$ = new VariableList(); }
          | var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
          | func_decl_args TCOMMA var_decl { $1->push_back($<var_decl>3); }
          ;

ident : TIDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
      ;

numeric : TINTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
        | TDOUBLE { $$ = new NDouble(atof($1->c_str())); delete $1; }
        ;
    
expr : ident TEQUAL expr { $$ = new NAssignment(*$<ident>1, *$3); }
     | ident TLPAREN call_args TRPAREN { $$ = new PSLang::NMethodCall(*$1, *$3); delete $3; }
     | ident { $<ident>$ = $1; }     
     | expr TPLUS expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | expr TMINUS expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | expr TMUL expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | expr TDIV expr { $$ = new NBinaryOperator(*$1, $2, *$3); } 
     | expr TCGT expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | expr TCLT expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | expr TCEQ expr { $$ = new NBinaryOperator(*$1, $2, *$3); }      
     | expr TCNE expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | expr TCLE expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | expr TCGE expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | expr TAND expr { $$ = new NBooleanOperator(*$1, $2, *$3); }
     | expr TOR expr { $$ = new NBooleanOperator(*$1, $2, *$3); }
     | TLPAREN expr TRPAREN { $$ = $2; }
     | numeric
     ;

			
	    
call_args : /*blank*/  { $$ = new ExpressionList(); }
          | expr { $$ = new ExpressionList(); $$->push_back($1); }
          | call_args TCOMMA expr  { $1->push_back($3); }
          ;
      
if_stmt : TIF TLPAREN expr TRPAREN block { $$ = new NIfStatement(*$3,*$5);}
		| TIF TLPAREN expr TRPAREN block TELSE block {$$ = new NIfElseStatement(*$3,*$5,*$7);}
		;
while_stmt: TWHILE TLPAREN expr TRPAREN block {$$ = new NWhileStatement(*$3,*$5);}
		  ;

%%

void PSLang::Parser::error( const PSLang::Parser::location_type &l,
                            const std::string &err_message)
{
    std::cerr << "Line: "<< l.begin.line <<std::endl
    		  << "Error: "<< err_message << "\n";
}

#include "Scanner.hpp"
static int yylex(PSLang::Parser::semantic_type *yylval,
                 PSLang::Scanner  &scanner,
                 PSLang::Driver   &driver)
{
   return( scanner.yylex(yylval) );
}
