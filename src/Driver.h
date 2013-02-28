#ifndef PSLANG_DRIVER_H
#define PSLANG_DRIVER_H 1
#include "Scanner.hpp"
#include "parser.hh"
#include "Expression.h"
#include <memory>
#include <map>
#include <string>
namespace PSLang{
    class Driver{
    public:
        Driver():parser(nullptr),scanner(nullptr),_isVerbose(false){};
        virtual ~Driver();
        void compile(const char *filename,const char* output);

        void setVerbose(const bool& value);

    private:

        std::unique_ptr<PSLang::Parser> parser;
        std::unique_ptr<PSLang::Scanner> scanner;
        bool _isVerbose;
    };
}

#endif //PSLANG_DRIVER_H
