#include <iostream>
#include <fstream>
#include <string>
#include "Driver.h"
#include "CodeGen.h"
#include "CodeGenLLVM.h"
#ifndef FAIL
#define FAIL -1
#endif

PSLang::Driver::~Driver() {
	delete programBlock;
}

void PSLang::Driver::setProgramBlock(NBlock* programBlock) {
	this->programBlock = programBlock;
}
void PSLang::Driver::setVerbose(const bool& value) {
	_isVerbose = value;
}
void PSLang::Driver::setLLVMMode(const bool& value)
{
	llvmMode = value;
}

void PSLang::Driver::compile(const std::string& filename,
		const std::string& outputFile) {
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

	CodeGenBase* codeGenerator;
	if(llvmMode)
	{
		codeGenerator = new CodeGenLLVM(outputFile);
	}
	else
	{
		codeGenerator = new CodeGen(outputFile);
	}

	codeGenerator->generateCode(*programBlock);

	if (_isVerbose) {
		std::cout << "Compilation ended successfully!" << std::endl;
	}

}
