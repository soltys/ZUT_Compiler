#include <iostream>
#include <fstream>
#include "Driver.h"

#ifndef FAIL
#define FAIL -1
#endif

PSLang::Driver::~Driver()
{
//    if(scanner != nullptr) delete(scanner);
//    if(parser != nullptr) delete(parser);
}

void PSLang::Driver::compile(const char *filename, const char* outputfile)
{
    std::ifstream in_file( filename );
       if( ! in_file.good() ) exit( EXIT_FAILURE );
       scanner = std::unique_ptr<PSLang::Scanner>(new PSLang::Scanner( &in_file ));
       /* check to see if its initialized */

       parser = std::unique_ptr<PSLang::Parser>(new PSLang::Parser( (*scanner) /* scanner */,
                                   	   	   	   (*this) /* driver */ ));

       if(parser->parse() == FAIL)
       {
          std::cerr << "Parse failed!!\n";
       }
}
