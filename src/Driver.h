#ifndef PSLANG_DRIVER_H
#define PSLANG_DRIVER_H 1
#include "Scanner.hpp"
#include "parser.hh"
#include <memory>
namespace PSLang{
    class Driver{
    public:
        Driver():parser(nullptr),scanner(nullptr){};
        virtual ~Driver();
        void compile(const char *filename,const char* output);

        void setVerbose(const bool& value);
    private:
        bool _isVerbose;
        std::unique_ptr<PSLang::Parser> parser;
        std::unique_ptr<PSLang::Scanner> scanner;
    };
}

#endif //PSLANG_DRIVER_H
