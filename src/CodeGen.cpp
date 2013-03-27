#include <iostream>
#include <sstream>
#include <stdexcept>
#include "Node.h"
#include "Instruction.h"
#include "Symbol.h"
#include "CodeGen.h"
#include "Utils.hpp"
#include "parser.hh"
#include <algorithm>
namespace PSLang {
typedef PSLang::Parser::token token;

std::ostream& operator <<(std::ostream& o, const Instruction& a) {
	o << a.instruction << std::endl;
	return o;
}

PSLang::Variable CodeGenContext::createVariable(std::string & name,
						PSLang::SymbolType type, bool isTemporary) {

  if (locals.find(name) == std::end(locals)) {
    int maxMemoryIndex = -1;
		for (auto it = std::begin(locals); it != std::end(locals); it++) {
			maxMemoryIndex = std::max(maxMemoryIndex, it->second->offset);
		}

		if (isTemporary) {
			locals.insert(
					std::make_pair(name,
							Variable_ptr(new TemporaryVariable(++maxMemoryIndex, type))));
		} else {
			locals.insert(
					std::make_pair(name,
							Variable_ptr(new Variable(++maxMemoryIndex, type))));
		}

		return Variable(maxMemoryIndex, type);
	} else {
		throw std::runtime_error(
				"Variable already exists!, could not create new one.");
	}

}
PSLang::Variable CodeGenContext::createTemporaryVariable(
		PSLang::SymbolType type) {
	_temporaryVariableCounter++;
	std::string proposeName = std::string(
			"__temporary" + toString(_temporaryVariableCounter));
	return createVariable(proposeName, type, true);
}
void CodeGenContext::generateCode(NBlock &root) {
	std::cout << "Generating code...\n";
	root.accept(*this);
	for (auto it = programInstructions.begin(); it != programInstructions.end();
	     it++) {
	  outputStream << *it;
	}

	std::cout << "Code is generated.\n";
	std::cout << "Number of instructions: "<< Instruction::_instuctionCounter << std::endl;
	std::cout << "-------------- Local variables ---------" << std::endl;
	for (auto it = locals.begin(); it != locals.end();	it++) {
			std::cout << it->first<< "\t\t" << it->second->getValue() <<std::endl;
		}
}


}
