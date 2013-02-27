#include <iostream>
#include <fstream>
#include "Driver.h"

PSLang::Driver::~Driver()
{
//    if(scanner != nullptr) delete(scanner);
//    if(parser != nullptr) delete(parser);
}

void PSLang::Driver::parse(const char *filename)
{
    std::cout << "Driver - parse - passed filename=" << filename;
}
