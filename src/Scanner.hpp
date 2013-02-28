#ifndef PSLANG_SCANNER_HPP
#define PSLANG_SCANNER_HPP 1

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL int PSLang::Scanner::yylex()

#include "parser.hh"

namespace PSLang{
    class Scanner : public yyFlexLexer{
    public:

    Scanner(std::istream *in):yyFlexLexer(in),yylval(new PSLang::Parser::semantic_type),lineNumber(0)
                              {};

    int yylex(PSLang::Parser::semantic_type *lval)
    {
        yylval = lval;
        return( yylex() );

    }
    void incrementLineNumber()
    {
    	lineNumber++;
    }

    int getLineNumber()
    {
    	return lineNumber;
    }

    private:

    int yylex();
    PSLang::Parser::semantic_type *yylval;
    int lineNumber;
};
}

#endif//PSLANG_SCANNER_HPP
