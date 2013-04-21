#include <iostream>
#include <cstdlib>
#include <string>
#include "Driver.h"
#include "main.h"


int main(const int argc, const char** argv) {

	std::string inputfile = "input.psl";

	if (argc > 3) {
		printHelp();
		return EXIT_FAILURE;
	}

	if(argc == 2){
		inputfile = argv[1];
	}

	std::string outputFilename = "app.asm";
	if (argc == 3) {
		outputFilename = argv[2];
	}

	PSLang::Driver driver;
	driver.setVerbose(true);
	driver.compile(inputfile, outputFilename);

	return EXIT_SUCCESS;
}

static void printHelp() {
	std::cout << "PSLang usage:" << std::endl;
	std::cout << "pslang [input] [output]" << std::endl;
}
