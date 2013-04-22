#include <iostream>
#include <cstdlib>
#include <string>
#include "Driver.h"
#include "main.h"
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
static int compileToLLVM = 0;
static int verboseMode = 0;
static std::string inputFile = "input.psl";
static std::string outputFilename = "app.asm";
int main(const int argc, const char** argv) {

	parseArguments(argc, argv);
	PSLang::Driver driver;
	driver.setVerbose(true);
	driver.setLLVMMode(compileToLLVM);
	driver.compile(inputFile, outputFilename);

	return EXIT_SUCCESS;
}

static void parseArguments(const int& argc, const char** argv) {
	int c;
	opterr = 0;
	static struct option long_options[] = { { "llvm", no_argument,
			&compileToLLVM, 1 }, { "verbose", no_argument, &verboseMode, 1 },
			{ 0, 0, 0, 0 } };

	int option_index = 0;
	while ((c = getopt_long(argc, (char **) argv, "i:o:", long_options,
			&option_index)) != -1)
		switch (c) {
		case 0:
			break;
		case 'i':
			inputFile = optarg;
			break;
		case 'o':
			outputFilename = optarg;
			break;
		case 'h':
			printHelp();
			exit(0);
		default:
			printHelp();
			exit(1);
			break;

		}

}

static void printHelp() {
	std::cout << "PSLang usage:" << std::endl;
	std::cout << "pslang [-i input_file] [-o output_file] [--llvm]"
			<< std::endl;
	std::cout << "-------------------------------" << std::endl;
	std::cout << "Defaults values:" << std::endl;
	std::cout << "\tinput_file = input.asm" << std::endl;
	std::cout << "\toutput_file = app.asm" << std::endl;
	std::cout << "\tllvm is set false" << std::endl;
}
