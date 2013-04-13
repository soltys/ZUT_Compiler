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

	if (context.locals.find(name) == std::end(context.locals)) {
		throw std::runtime_error("Variable is not declared");
	}
	Variable_ptr var = context.locals.find(name)->second;
	context.valueStack.push(var);
}

void NMethodCall::accept(CodeGenContext& context) {

}

void NBinaryOperator::accept(CodeGenContext& context) {
	rhs.accept(context);
	lhs.accept(context);

	Symbol_ptr lhsValue = context.valueStack.top();
	context.valueStack.pop();

	Symbol_ptr rhsValue = context.valueStack.top();
	context.valueStack.pop();

	PSLang::SymbolType resultType =
			lhsValue->getType() == Float || rhsValue->getType() == Float ?
					Float : Int;

	std::string registerType = resultType == Int ? "R" : "F";

	context.programInstructions.push_back(
			Instruction("MOV", registerType + "2", rhsValue->getValue()));
	context.programInstructions.push_back(
			Instruction("MOV", registerType + "1", lhsValue->getValue()));

	std::function<void(std::string)> genBool =
			[&](std::string cmd) {
				context.programInstructions.push_back(Instruction("SUB", registerType + "1", registerType + "2"));
				context.programInstructions.push_back(Instruction(cmd, toString(Instruction::_instuctionCounter +3)));
				context.programInstructions.push_back(Instruction("MOV", registerType + "1", "0"));
				context.programInstructions.push_back(Instruction("JMP", toString(Instruction::_instuctionCounter +2)));
				context.programInstructions.push_back(Instruction("MOV", registerType + "1", "1"));
			};
	switch (op) {
	case token::TPLUS:
		context.programInstructions.push_back(
				Instruction("ADD", registerType + "1", registerType + "2"));
		break;
	case token::TMINUS:
		context.programInstructions.push_back(
				Instruction("SUB", registerType + "1", registerType + "2"));
		break;
	case token::TMUL:
		context.programInstructions.push_back(
				Instruction("MUL", registerType + "1", registerType + "2"));
		break;
	case token::TDIV:
		context.programInstructions.push_back(
				Instruction("DIV", registerType + "1", registerType + "2"));
		break;

	case token::TCGT: // >
		genBool("JG");
		break;
	case token::TCGE: // >=
		genBool("JNL");
		break;
	case token::TCLT: // <
		genBool("JL");
		break;
	case token::TCLE: // <=
		genBool("JNG");
		break;
	case token::TCNE: // !=
		genBool("JNZ");
		break;
	case token::TCEQ: // ==
		genBool("JZ");
		break;
	}
	PSLang::Variable var = context.createTemporaryVariable(resultType);
	context.programInstructions.push_back(
			Instruction("MOV", var.getValue(), registerType + "1"));
	context.valueStack.push(
			std::shared_ptr<PSLang::Variable>(new PSLang::Variable(var)));

}

void NBooleanOperator::accept(CodeGenContext& context) {
	rhs.accept(context);
	lhs.accept(context);

	if (op == token::TAND) {
		operatorAnd(context);
	} else if (op == token::TOR) {
		operatorOr(context);
	} else {
		throw std::runtime_error("unsuported boolean operator");
	}

}

void NBooleanOperator::operatorAnd(CodeGenContext& context) {
	Symbol_ptr lhsValue = context.valueStack.top();
	context.valueStack.pop();

	Symbol_ptr rhsValue = context.valueStack.top();
	context.valueStack.pop();

	PSLang::SymbolType resultType =
			lhsValue->getType() == Float || rhsValue->getType() == Float ?
					Float : Int;

	std::string registerType = resultType == Int ? "R" : "F";

	context.programInstructions.push_back(
			Instruction("MOV", registerType + "1", lhsValue->getValue()));
	context.programInstructions.push_back(
			Instruction("JZ", toString(Instruction::_instuctionCounter + 5)));
	context.programInstructions.push_back(
			Instruction("MOV", registerType + "2", rhsValue->getValue()));
	context.programInstructions.push_back(
			Instruction("JZ", toString(Instruction::_instuctionCounter + 3)));
	context.programInstructions.push_back(
			Instruction("MOV", registerType + "1", "1"));
	context.programInstructions.push_back(
			Instruction("JMP", toString(Instruction::_instuctionCounter + 2)));
	context.programInstructions.push_back(
			Instruction("MOV", registerType + "1", "0"));

	PSLang::Variable var = context.createTemporaryVariable(resultType);
	context.programInstructions.push_back(
			Instruction("MOV", var.getValue(), registerType + "1"));
	context.valueStack.push(
			std::shared_ptr<PSLang::Variable>(new PSLang::Variable(var)));
}

void NBooleanOperator::operatorOr(CodeGenContext& context) {
	Symbol_ptr lhsValue = context.valueStack.top();
	context.valueStack.pop();

	Symbol_ptr rhsValue = context.valueStack.top();
	context.valueStack.pop();

	PSLang::SymbolType resultType =
			lhsValue->getType() == Float || rhsValue->getType() == Float ?
					Float : Int;

	std::string registerType = resultType == Int ? "R" : "F";

	context.programInstructions.push_back(
			Instruction("MOV", registerType + "1", lhsValue->getValue()));
	context.programInstructions.push_back(
			Instruction("JNZ", toString(Instruction::_instuctionCounter + 5)));
	context.programInstructions.push_back(
			Instruction("MOV", registerType + "2", rhsValue->getValue()));
	context.programInstructions.push_back(
			Instruction("JNZ", toString(Instruction::_instuctionCounter + 3)));
	context.programInstructions.push_back(
			Instruction("MOV", registerType + "1", "0"));
	context.programInstructions.push_back(
			Instruction("JMP", toString(Instruction::_instuctionCounter + 2)));
	context.programInstructions.push_back(
			Instruction("MOV", registerType + "1", "1"));

	PSLang::Variable var = context.createTemporaryVariable(resultType);
	context.programInstructions.push_back(
			Instruction("MOV", var.getValue(), registerType + "1"));
	context.valueStack.push(
			std::shared_ptr<PSLang::Variable>(new PSLang::Variable(var)));
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
		context.clearTemporaryVariables();
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
	} else if (type.name == "float") {
		context.createVariable(id.name, Float);
	} else {
		throw std::runtime_error("Supported only types of int & float");
	}

	if (assignmentExpression != nullptr) {
		NAssignment assn(id, *assignmentExpression);
		assn.accept(context);
	}

}

void NFunctionDeclaration::accept(CodeGenContext& context) {

}

void NIfStatement::accept(CodeGenContext& context) {
	boolExpr.accept(context);
	Symbol_ptr var = context.valueStack.top();
	context.programInstructions.push_back(
			Instruction("MOV", var->getTypeRegister() + "1", var->getValue()));
	context.valueStack.pop();

	auto labelName = context.addJumpWithLabel("JZ",
			Instruction::_instuctionCounter);

	block.accept(context);
	context.createLabel(labelName, Instruction::_instuctionCounter);
}

void NIfElseStatement::accept(CodeGenContext& context) {
	boolExpr.accept(context);
	Symbol_ptr var = context.valueStack.top();
	context.programInstructions.push_back(
			Instruction("MOV", var->getTypeRegister() + "1", var->getValue()));
	context.valueStack.pop();

	auto ifLabel = context.addJumpWithLabel("JZ",
			Instruction::_instuctionCounter);

	block.accept(context);
	auto endIfLabel = context.addJumpWithLabel("JMP",
			Instruction::_instuctionCounter);
	context.createLabel(ifLabel, Instruction::_instuctionCounter);
	elseBlock.accept(context);
	context.createLabel(endIfLabel, Instruction::_instuctionCounter);
}

void NWhileStatement::accept(CodeGenContext& context) {
	int beginWhile = Instruction::_instuctionCounter;
	boolExpr.accept(context);
	Symbol_ptr var = context.valueStack.top();
	context.programInstructions.push_back(
			Instruction("MOV", var->getTypeRegister() + "1", var->getValue()));
	context.valueStack.pop();

	auto labelName = context.addJumpWithLabel("JZ",
			Instruction::_instuctionCounter);

	block.accept(context);
	auto beginWhileLabel = context.addJumpWithLabel("JMP",	beginWhile);

	context.createLabel(labelName, Instruction::_instuctionCounter);
	context.createLabel(beginWhileLabel, beginWhile);
}

void NForStatement::accept(CodeGenContext& context)
{
	varDecl.accept(context);
	int beginWhile = Instruction::_instuctionCounter;
		boolExpr.accept(context);
		Symbol_ptr var = context.valueStack.top();
		context.programInstructions.push_back(
				Instruction("MOV", var->getTypeRegister() + "1", var->getValue()));
		context.valueStack.pop();

		auto labelName = context.addJumpWithLabel("JZ",
				Instruction::_instuctionCounter);

		block.accept(context);
		exprStmt.accept(context);
		auto beginWhileLabel = context.addJumpWithLabel("JMP",	beginWhile);

		context.createLabel(labelName, Instruction::_instuctionCounter);
		context.createLabel(beginWhileLabel, beginWhile);
}

}
