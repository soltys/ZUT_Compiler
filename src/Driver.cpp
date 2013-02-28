#include <iostream>
#include <fstream>
#include "Driver.h"

#ifndef FAIL
#define FAIL -1
#endif

PSLang::Driver::~Driver() {
}
void PSLang::Driver::setVerbose(const bool& value) {
	_isVerbose = value;
}
void PSLang::Driver::compile(const char *filename, const char* outputFile) {
	if (_isVerbose) {
	std::cout << "Compilation started!" << std::endl << "Input file: "
			<< filename << std::endl << "Output file: " << outputFile
			<< std::endl;
	}
	std::ifstream in_file(filename);
	if (!in_file.good())
		exit(EXIT_FAILURE);
	scanner = std::unique_ptr<PSLang::Scanner>(new PSLang::Scanner(&in_file));
	parser = std::unique_ptr<PSLang::Parser>(
			new PSLang::Parser((*scanner) /* scanner */, (*this) /* driver */));

	if (parser->parse() == FAIL) {
		std::cerr << "Parse failed!!\n";
	}

	if(_isVerbose){
		std::cout << "Compilation ended successfully!" << std::endl;
	}

}
