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

void NInteger::accept(CodeGenContext& context) {
	context.valueStack.push(
			std::shared_ptr<IntConstant>(new IntConstant(value)));
}

void NDouble::accept(CodeGenContext& context) {
	context.valueStack.push(
			std::shared_ptr<FloatConstant>(new FloatConstant(value)));
}

void NIdentifier::accept(CodeGenContext& context) {

}

void NMethodCall::accept(CodeGenContext& context) {

}

void NBinaryOperator::accept(CodeGenContext& context) {
	rhs.accept(context);
	lhs.accept(context);

	Symbol_ptr lhsValue = context.valueStack.top();
	context.programInstructions.push_back(
			Instruction("MOV", "R1", lhsValue->getValue()));
	context.valueStack.pop();

	Symbol_ptr rhsValue = context.valueStack.top();
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
	Variable_ptr var = context.locals.find(lhs.name)->second;
	if (context.locals.find(lhs.name) == std::end(context.locals)) {
		throw std::runtime_error("Variable is not declared");
	}
	rhs.accept(context);



	std::shared_ptr<Symbol> rhsValue = context.valueStack.top();
	context.programInstructions.push_back(
			Instruction("MOV", var->getValue(), rhsValue->getValue()));
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

void NIfStatement::accept(CodeGenContext& context){
	std::cout <<"Generating code for if statement" <<std::endl;
}

void NWhileStatement::accept(CodeGenContext& context){
	std::cout <<"Generating code for if statement" <<std::endl;
}

}
