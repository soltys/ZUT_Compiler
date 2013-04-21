#ifndef PSLANG_DRIVER_H
#define PSLANG_DRIVER_H 1
#include "Scanner.hpp"
#include "parser.hh"
#include "Node.h"
#include <memory>
#include <map>
#include <string>
namespace PSLang {
class Driver {
public:
	Driver() :
			programBlock(nullptr),parser(nullptr), scanner(nullptr), _isVerbose(false){
	}

	virtual ~Driver();
	void compile(const std::string& filename, const std::string& output);

	/**
	 * Sets compilation into verbose mode.
	 */
	void setVerbose(const bool& value);

	void setProgramBlock(NBlock* programBlock);

private:
	NBlock* programBlock;
	std::unique_ptr<PSLang::Parser> parser;
	std::unique_ptr<PSLang::Scanner> scanner;
	bool _isVerbose;
};
}

#endif //PSLANG_DRIVER_H
