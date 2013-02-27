#include <iostream>
#include <cstdlib>

#include "Driver.h"

int main(const int argc, const char** argv)
{
    PSLang::Driver driver;

    driver.parse("Hardcoded string\n");
    return 0;
}
