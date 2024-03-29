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

	if (a.param2.length() > 0) {
		o << "," << a.param2;
	}
	o << std::endl;
	return o;
}

PSLang::Variable CodeGen::createArray(std::string& name,
		PSLang::SymbolType type, std::vector<long int> indexes) {
	if (locals.find(name) != std::end(locals)) {
		throw std::runtime_error(
				"Variable already exists!, could not create new one.");
	}
	int maxMemoryIndex = 0;
	for (auto it = std::begin(locals); it != std::end(locals); it++) {
		maxMemoryIndex = std::max(maxMemoryIndex,
				it->second->offset + it->second->getSize());
	}
	locals.insert(
			std::make_pair(name,
					Variable_ptr(new Variable(maxMemoryIndex, indexes, type))));

	return Variable(maxMemoryIndex, indexes, type);

}

PSLang::Variable CodeGen::createVariable(std::string & name,
		PSLang::SymbolType type, bool isTemporary, int size) {

	if (locals.find(name) == std::end(locals)) {
		int maxMemoryIndex = 0;
		for (auto it = std::begin(locals); it != std::end(locals); it++) {
			maxMemoryIndex = std::max(maxMemoryIndex,
					it->second->offset + it->second->getSize());
		}

		if (isTemporary) {
			locals.insert(
					std::make_pair(name,
							Variable_ptr(
									new TemporaryVariable(maxMemoryIndex,
											type))));
		} else {
			locals.insert(
					std::make_pair(name,
							Variable_ptr(
									new Variable(maxMemoryIndex, size, type))));
		}

		return Variable(maxMemoryIndex, size, type);
	} else {
		throw std::runtime_error(
				"Variable already exists!, could not create new one.");
	}

}
PSLang::Variable CodeGen::createTemporaryVariable(
		PSLang::SymbolType type) {
	_temporaryVariableCounter++;
	std::string proposeName = std::string(
			"__temporary" + toString(_temporaryVariableCounter));
	return createVariable(proposeName, type, true);
}

void CodeGen::clearTemporaryVariables() {
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

Variable_ptr CodeGen::getVariable(const std::string& name) {
	if (locals.find(name) == std::end(locals)) {
		throw std::runtime_error("Variable is not declared");
	}
	Variable_ptr var = locals.find(name)->second;
	return var;
}

std::string CodeGen::addJumpWithLabel(const std::string& command) {
	int id = Instruction::_instuctionCounter;
	std::string labelName = "__label-" + toString(id);
	programInstructions.push_back(Instruction(command, labelName));
	return labelName;
}

void CodeGen::createLabel(const std::string& name, const int& value) {
	labels.insert(std::make_pair(name, value));
}
void CodeGen::clearLabels() {
	auto begin = std::begin(labels);
	auto end = std::end(labels);
	labels.erase(begin, end);
}

void CodeGen::swapLabels() {
	for (PSLang::Instruction& instruction : programInstructions) {
		if (boost::starts_with(instruction.getParam1(), "__label")) {
			int ip = labels.find(instruction.getParam1())->second;
			if (labels.find(instruction.getParam1()) == std::end(labels)) {
				throw std::runtime_error(
						"Label" + instruction.getParam1() + " is not found :(");
			}
			instruction.setParam1(toString(ip));
		}
	}
}

void CodeGen::createFunction(const std::string& name,
		int instuctionStart) {
	if (functions.find(name) != std::end(functions)) {
		throw std::runtime_error(
				"Function already exists!, could not create new one.");
	}
	functions.insert(std::make_pair(name, instuctionStart));
}

void CodeGen::addValueStackSymbol(Symbol_ptr symbol)
{
	valueStack.push(symbol);
}

Symbol_ptr CodeGen::getSymbolFromValueStack()
{
	Symbol_ptr symbol = valueStack.top();
	valueStack.pop();
	return symbol;
}

void CodeGen::addInstruction(const std::string& cmd,
		const std::string& param1) {
	programInstructions.push_back(Instruction(cmd, param1));
}
void CodeGen::addInstruction(const std::string& cmd,
		const std::string& param1, const std::string& param2) {
	programInstructions.push_back(Instruction(cmd, param1, param2));
}

void CodeGen::generateCode(NBlock &root) {
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
	std::cout << "CURRENT SIZE: " << valueStack.size() << std::endl;

	std::cout << "--------------    Functions   ---------" << std::endl;
		for (auto it = functions.begin(); it != functions.end(); it++) {
			std::cout << it->first << "\t\t" << it->second << std::endl;
		}

	std::cout << "--------------    LABELS   ---------" << std::endl;
	for (auto it = labels.begin(); it != labels.end(); it++) {
		std::cout << it->first << "\t\t" << it->second << std::endl;
	}
}

}
