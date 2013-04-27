/*
 * CodeGenLLVM.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: soltys
 */

#include "CodeGenLLVM.h"
#include "Node.h"
#include "parser.hh"
#include <iostream>
#include <string>
using namespace llvm;
using namespace std;
namespace PSLang {
typedef PSLang::Parser::token token;
CodeGenLLVM::CodeGenLLVM(std::string fileName) :
		builder(getGlobalContext()) {

}

CodeGenLLVM::~CodeGenLLVM() {
	// TODO Auto-generated destructor stub
}

llvm::Value* CodeGenLLVM::getVariable(const std::string& name)  {
	return namedValues[name];
}

void CodeGenLLVM::generateCode(NBlock& root) {
	std::cout << "Generating code...\n";

	std::cout << "Code is generated.\n";

}

Value* NInteger::codeGen(CodeGenLLVM& context) {
	return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value, true);
}

Value* NDouble::codeGen(CodeGenLLVM& context) {
	return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), value);
}
Value* NArrayIdentifier::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}
Value* NIdentifier::codeGen(CodeGenLLVM& context) {
	Value *V = context.getVariable(name);
	return V ? V : nullptr;
}

Value* NMethodCall::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NBinaryOperator::codeGen(CodeGenLLVM& context) {
	Value *L = lhs.codeGen(context);
	Value *R = rhs.codeGen(context);
	if (L == nullptr || R == nullptr)
		return nullptr;

	switch (op) {
	case token::TPLUS:
		return context.builder.CreateFAdd(L, R, "addtmp");
	case token::TMINUS:
		return context.builder.CreateFSub(L, R, "subtmp");
	case token::TMUL:
		return context.builder.CreateFMul(L, R, "multmp");
	case token::TCLT:
		L = context.builder.CreateFCmpULT(L, R, "cmptmp");
		// Convert bool 0/1 to double 0.0 or 1.0
		return context.builder.CreateUIToFP(L,
				Type::getDoubleTy(getGlobalContext()), "booltmp");
	default:
		return (nullptr);
	}
}

Value* NAssignment::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NBlock::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NExpressionStatement::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NArrayDeclaration::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NVariableDeclaration::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NFunctionDeclaration::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NWhileStatement::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}
Value* NForStatement::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NIfStatement::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NIfElseStatement::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NBooleanOperator::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}
} /* namespace PSLang */
