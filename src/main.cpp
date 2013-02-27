#include <iostream>
#include <cstdlib>

#include "Driver.h"


static void printHelp()
{
	std::cout << "PSLang usage:" <<std::endl;
    std::cout << "pslang input [output]" <<std::endl;
}

int main(const int argc, const char** argv)
{
	if(argc == 1 || argc > 3)
	{
		printHelp();
		return EXIT_FAILURE;
	}

    PSLang::Driver driver;
    driver.compile(argv[1],"compiled");

    return EXIT_SUCCESS;
}

