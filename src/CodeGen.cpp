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
	o << a.command << " " << a.param1;

	if (a.param2.length() > 0)
	{
		o << "," << a.param2;
	}
	o << std::endl;
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
							Variable_ptr(
									new TemporaryVariable(++maxMemoryIndex,
											type))));
		} else {
			locals.insert(
					std::make_pair(name,
							Variable_ptr(
									new Variable(++maxMemoryIndex, type))));
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

void CodeGenContext::clearTemporaryVariables() {
	auto iter = locals.begin();
	auto endIter = locals.end();
	for (; iter != endIter;) {
		if (iter->second->isTemporaryValue()) {
			locals.erase(iter++);
		} else {
			++iter;
		}
	}
}
void CodeGenContext::createLabel(const std::string& name, const int& value){
	labels.insert(std::make_pair(name,value));
}
void CodeGenContext::clearLabels() {
	auto begin = std::begin(labels);
	auto end = std::end(labels);
	labels.erase(begin,end);
}

void CodeGenContext::swapLabels(){
	for(PSLang::Instruction& instruction: programInstructions)
	{
		if(boost::starts_with(instruction.param1,"__label"))
		{
			int ip = labels.find(instruction.param1)->second;
				if (labels.find(instruction.param1) == std::end(labels)) {
					throw std::runtime_error("Label" + instruction.param1 +  " is not found :(");
				}
			instruction.param1 = toString(ip);
		}
	}
}

void CodeGenContext::generateCode(NBlock &root) {
	std::cout << "Generating code...\n";
	root.accept(*this);
	swapLabels();
	for (auto it = programInstructions.begin(); it != programInstructions.end();
			it++) {
		outputStream << *it;
	}

	std::cout << "Code is generated.\n";
	std::cout << "Number of instructions: " << Instruction::_instuctionCounter
			<< std::endl;
	std::cout << "-------------- Local variables ---------" << std::endl;
	for (auto it = locals.begin(); it != locals.end(); it++) {
		std::cout << it->first << "\t\t" << it->second->getValue() << std::endl;
	}

	std::cout << "-------------- Value stack ---------" << std::endl;
	std::cout << "CURRENT SIZE: " << valueStack.size() <<  std::endl;

}

}
