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
#include <boost/utility.hpp>
namespace PSLang {
typedef PSLang::Parser::token token;

void NInteger::accept(CodeGenContext& context) {
	context.addValueStackSymbol(
			std::shared_ptr<IntConstant>(new IntConstant(value)));
}

void NDouble::accept(CodeGenContext& context) {
	context.addValueStackSymbol(
			std::shared_ptr<FloatConstant>(new FloatConstant(value)));
}

void NIdentifier::accept(CodeGenContext& context) {

	Variable_ptr var = context.getVariable(name);
	context.addValueStackSymbol(var);
}

void NArrayIdentifier::accept(CodeGenContext& context) {

	std::cout << "Array indent" << std::endl;

	Variable_ptr var = context.getVariable(name);
	context.addInstruction("MOV","R0", toString(var->offset));
	NExpression* arrayOffset = indexes.back();
	arrayOffset->accept(context);
	Symbol_ptr arrayOffsetValue = context.getSymbolFromValueStack();
	context.addInstruction("ADD", "R0", arrayOffsetValue->getValue());

	if (indexes.size() > 1) {
		auto varIt = var->indexes.begin();
		++varIt;
		for (auto it = indexes.begin();
				(it != indexes.end()) && (boost::next(it) != indexes.end());
				++it) {
			for (auto arrayIt = varIt; arrayIt != var->indexes.end();
					++arrayIt) {
				std::shared_ptr<NExpression> integer = std::shared_ptr<NExpression>(new NInteger(*arrayIt));
				auto binaryOp = NBinaryOperator(*integer,token::TMUL,**it);
				binaryOp.accept(context);
				Symbol_ptr mulValue = context.getSymbolFromValueStack();
					context.addInstruction("ADD", "R0", mulValue->getValue());

			}
			++varIt;
		}

	}

	auto stackValue = Symbol_ptr(new Address(SymbolType::Int));
	context.addValueStackSymbol(stackValue);

}

void NMethodCall::accept(CodeGenContext& context) {
	if (context.functions.find(id.name) == std::end(context.functions)) {
		throw std::runtime_error("Function is not declared");
	}
	int functionStart = context.functions.find(id.name)->second;
	auto labelName = context.addJumpWithLabel("PUSH");
	for (auto it = arguments.begin(); it != arguments.end(); ++it) {
		auto expr = *it;
		expr->accept(context);
		Symbol_ptr value = context.getSymbolFromValueStack();
		context.addInstruction("PUSH", value->getValue());
	}
	context.addInstruction("JMP", toString(functionStart));
	context.createLabel(labelName,Instruction::_instuctionCounter);

}

void NBinaryOperator::accept(CodeGenContext& context) {
	rhs.accept(context);
	lhs.accept(context);

	Symbol_ptr lhsValue = context.getSymbolFromValueStack();

	Symbol_ptr rhsValue =context.getSymbolFromValueStack();

	PSLang::SymbolType resultType =
			lhsValue->getType() == Float || rhsValue->getType() == Float ?
					Float : Int;

	std::string registerType = resultType == Int ? "R" : "F";

	context.addInstruction("MOV", registerType + "2", rhsValue->getValue());
	context.addInstruction("MOV", registerType + "1", lhsValue->getValue());

	std::function<void(std::string)> genBool =
			[&](std::string cmd) {
				context.addInstruction("SUB", registerType + "1", registerType + "2");
				context.addInstruction(cmd, toString(Instruction::_instuctionCounter +3));
				context.addInstruction("MOV", registerType + "1", "0");
				context.addInstruction("JMP", toString(Instruction::_instuctionCounter +2));
				context.addInstruction("MOV", registerType + "1", "1");
			};
	switch (op) {
	case token::TPLUS:
		context.addInstruction("ADD", registerType + "1", registerType + "2");
		break;
	case token::TMINUS:
		context.addInstruction("SUB", registerType + "1", registerType + "2");
		break;
	case token::TMUL:
		context.addInstruction("MUL", registerType + "1", registerType + "2");
		break;
	case token::TDIV:
		context.addInstruction("DIV", registerType + "1", registerType + "2");
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
	context.addInstruction("MOV", var.getValue(), registerType + "1");
	context.addValueStackSymbol(
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
	Symbol_ptr lhsValue = context.getSymbolFromValueStack();

	Symbol_ptr rhsValue = context.getSymbolFromValueStack();

	PSLang::SymbolType resultType =
			lhsValue->getType() == Float || rhsValue->getType() == Float ?
					Float : Int;

	std::string registerType = resultType == Int ? "R" : "F";

	context.addInstruction("MOV", registerType + "1", lhsValue->getValue());
	context.addInstruction("JZ", toString(Instruction::_instuctionCounter + 5));
	context.addInstruction("MOV", registerType + "2", rhsValue->getValue());
	context.addInstruction("JZ", toString(Instruction::_instuctionCounter + 3));
	context.addInstruction("MOV", registerType + "1", "1");
	context.addInstruction("JMP", toString(Instruction::_instuctionCounter + 2));
	context.addInstruction("MOV", registerType + "1", "0");

	PSLang::Variable var = context.createTemporaryVariable(resultType);
	context.addInstruction("MOV", var.getValue(), registerType + "1");
	context.addValueStackSymbol(
			std::shared_ptr<PSLang::Variable>(new PSLang::Variable(var)));
}

void NBooleanOperator::operatorOr(CodeGenContext& context) {
	Symbol_ptr lhsValue = context.getSymbolFromValueStack();

	Symbol_ptr rhsValue = context.getSymbolFromValueStack();

	PSLang::SymbolType resultType =
			lhsValue->getType() == Float || rhsValue->getType() == Float ?
					Float : Int;

	std::string registerType = resultType == Int ? "R" : "F";

	context.addInstruction("MOV", registerType + "1", lhsValue->getValue());
	context.addInstruction("JNZ", toString(Instruction::_instuctionCounter + 5));
	context.addInstruction("MOV", registerType + "2", rhsValue->getValue());
	context.addInstruction("JNZ", toString(Instruction::_instuctionCounter + 3));
	context.addInstruction("MOV", registerType + "1", "0");
	context.addInstruction("JMP", toString(Instruction::_instuctionCounter + 2));
	context.addInstruction("MOV", registerType + "1", "1");

	PSLang::Variable var = context.createTemporaryVariable(resultType);
	context.addInstruction("MOV", var.getValue(), registerType + "1");
	context.addValueStackSymbol(
			std::shared_ptr<PSLang::Variable>(new PSLang::Variable(var)));
}
void NAssignment::accept(CodeGenContext& context) {

	std::cout << "Creating assignment for " << lhs.name << std::endl;
	Variable_ptr var = context.getVariable(lhs.name);

	rhs.accept(context);
	std::shared_ptr<Symbol> rhsValue = context.getSymbolFromValueStack();

	lhs.accept(context);
	std::shared_ptr<Symbol> lhsVariable = context.getSymbolFromValueStack();
	context.addInstruction("MOV", lhsVariable->getValue(), rhsValue->getValue());
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

void NArrayDeclaration::accept(CodeGenContext& context) {
	std::stringstream ss;

	int arraySize = 1;
	std::cout << "Creating array declaration " << type.name << " " << id.name;
	for (auto& index : indexes) {
		std::cout << "[" << arraySize << "]";
		arraySize *= index;
	}
	std::cout << std::endl;

	if (type.name == "int") {
		context.createArray(id.name, Int, indexes);
	} else if (type.name == "float") {
		context.createArray(id.name, Float, indexes);
	} else {
		throw std::runtime_error("Supported only types of int & float");
	}

	if (assignmentExpression != nullptr) {
		NAssignment assn(id, *assignmentExpression);
		assn.accept(context);
	}

}

void NFunctionDeclaration::accept(CodeGenContext& context) {
	auto labelName = context.addJumpWithLabel("JMP");
	context.createFunction(id.name, Instruction::_instuctionCounter);
	for (auto it = arguments.begin(); it != arguments.end(); ++it) {
		auto var_indet = *it;
		var_indet->accept(context);
		Variable_ptr var = context.getVariable(var_indet->id.name);

		context.addInstruction("POP", var->getValue());
	}
	block.accept(context);
	context.addInstruction("POP", "R1");
	context.addInstruction("JMP", "R1");
	context.createLabel(labelName, Instruction::_instuctionCounter);
}

void NIfStatement::accept(CodeGenContext& context) {
	boolExpr.accept(context);
	Symbol_ptr var = context.getSymbolFromValueStack();
	context.addInstruction("MOV", var->getTypeRegister() + "1", var->getValue());


	auto labelName = context.addJumpWithLabel("JZ");
	block.accept(context);
	context.createLabel(labelName, Instruction::_instuctionCounter);
}

void NIfElseStatement::accept(CodeGenContext& context) {
	boolExpr.accept(context);
	Symbol_ptr var = context.getSymbolFromValueStack();
	context.addInstruction("MOV", var->getTypeRegister() + "1", var->getValue());


	auto ifLabel = context.addJumpWithLabel("JZ");

	block.accept(context);
	auto endIfLabel = context.addJumpWithLabel("JMP");
	context.createLabel(ifLabel, Instruction::_instuctionCounter);
	elseBlock.accept(context);
	context.createLabel(endIfLabel, Instruction::_instuctionCounter);
}

void NWhileStatement::accept(CodeGenContext& context) {
	int beginWhile = Instruction::_instuctionCounter;
	boolExpr.accept(context);
	Symbol_ptr var = context.getSymbolFromValueStack();
	context.addInstruction("MOV", var->getTypeRegister() + "1", var->getValue());

	auto labelName = context.addJumpWithLabel("JZ");

	block.accept(context);
	auto beginWhileLabel = context.addJumpWithLabel("JMP");

	context.createLabel(labelName, Instruction::_instuctionCounter);
	context.createLabel(beginWhileLabel, beginWhile);
}

void NForStatement::accept(CodeGenContext& context) {
	assigment.accept(context);
	int beginWhile = Instruction::_instuctionCounter;
	boolExpr.accept(context);
	Symbol_ptr var = context.getSymbolFromValueStack();
	context.addInstruction("MOV", var->getTypeRegister() + "1", var->getValue());


	auto labelName = context.addJumpWithLabel("JZ");

	block.accept(context);
	exprStmt.accept(context);
	auto beginWhileLabel = context.addJumpWithLabel("JMP");

	context.createLabel(labelName, Instruction::_instuctionCounter);
	context.createLabel(beginWhileLabel, beginWhile);
}

}
