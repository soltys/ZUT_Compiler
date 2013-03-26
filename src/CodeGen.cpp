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
			maxMemoryIndex = std::max(maxMemoryIndex, it->second.offset);
		}

		if (isTemporary) {
			locals.insert(
					std::make_pair(name,
							TemporaryVariable(++maxMemoryIndex, type)));
		} else {
			locals.insert(
					std::make_pair(name, Variable(++maxMemoryIndex, type)));
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
			std::cout << it->first<< "\t\t" << it->second.getValue() <<std::endl;
		}
}
std::string NInteger::getStringValue() {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

ValueType NInteger::getValueType() {
	return ValueType::IntType;
}
void NInteger::accept(CodeGenContext& context) {
	context.valueStack.push(
			std::shared_ptr<IntConstant>(new IntConstant(value)));
}

void NDouble::accept(CodeGenContext& context) {
	context.valueStack.push(
			std::shared_ptr<IntConstant>(new IntConstant(value)));
}

void NIdentifier::accept(CodeGenContext& context) {

}

void NMethodCall::accept(CodeGenContext& context) {

}

void NBinaryOperator::accept(CodeGenContext& context) {
	rhs.accept(context);
	lhs.accept(context);

	std::shared_ptr<Symbol> lhsValue = context.valueStack.top();
	context.programInstructions.push_back(
			Instruction("MOV", "R1", lhsValue->getValue()));
	context.valueStack.pop();

	std::shared_ptr<Symbol> rhsValue = context.valueStack.top();
	context.programInstructions.push_back(
			Instruction("MOV", "R2", rhsValue->getValue()));
	context.valueStack.pop();

	switch (op) {
	case token::TPLUS:
		context.programInstructions.push_back(Instruction("ADD", "R1", "R2"));
		break;
	case token::TMINUS:
		context.programInstructions.push_back(Instruction("SUB", "R1", "R2"));
		break;
	case token::TMUL:
		context.programInstructions.push_back(Instruction("MUL", "R1", "R2"));
		break;
	case token::TDIV:
		context.programInstructions.push_back(Instruction("DIV", "R1", "R2"));
		break;

	}
	PSLang::Variable var = context.createTemporaryVariable(Int);
	context.programInstructions.push_back(
			Instruction("MOV", var.getValue(), "R1"));
	context.valueStack.push(std::shared_ptr<PSLang::Variable>(new PSLang::Variable(var)));

}

void NAssignment::accept(CodeGenContext& context) {

	std::cout << "Creating assignment for " << lhs.name << std::endl;
	PSLang::Variable var = context.locals.find(lhs.name)->second;
	if (context.locals.find(lhs.name) == std::end(context.locals)) {
		throw std::runtime_error("Variable is not declared");
	}
	rhs.accept(context);



	std::shared_ptr<Symbol> rhsValue = context.valueStack.top();
	context.programInstructions.push_back(
			Instruction("MOV", var.getValue(), rhsValue->getValue()));
	context.valueStack.pop();
}

void NBlock::accept(CodeGenContext& context) {

	StatementList::const_iterator it;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cout << "Generating code for " << typeid(**it).name() << std::endl;
		(**it).accept(context);
	}

}

void NExpressionStatement::accept(CodeGenContext& context) {
	expression.accept(context);
}

void NVariableDeclaration::accept(CodeGenContext& context) {
	std::stringstream ss;
	std::cout << "Creating variable declaration " << type.name << " " << id.name
			<< std::endl;

	if (type.name == "int") {
		context.createVariable(id.name, Int);
	} else {
		context.createVariable(id.name, Float);
	}

	if (assignmentExpression != nullptr) {
		NAssignment assn(id, *assignmentExpression);
		assn.accept(context);
	}

}

void NFunctionDeclaration::accept(CodeGenContext& context) {

}

}
