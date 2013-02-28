#include <iostream>
#include <cstdlib>
#include <string>
#include "Driver.h"

static void printHelp() {
	std::cout << "PSLang usage:" << std::endl;
	std::cout << "pslang input [output]" << std::endl;
}

int main(const int argc, const char** argv) {
	if (argc == 1 || argc > 3) {
		printHelp();
		return EXIT_FAILURE;
	}

	std::string outputFilename = "a.out";
	if (argc == 3) {
		outputFilename = argv[2];
	}

	PSLang::Driver driver;
	driver.setVerbose(true);
	driver.compile(argv[1], outputFilename.c_str());

	return EXIT_SUCCESS;
}

