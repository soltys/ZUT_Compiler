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

#include <llvm/PassManagers.h>
#include <llvm/DefaultPasses.h>
#include <llvm/Instructions.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/raw_os_ostream.h>
using namespace llvm;
using namespace std;
namespace PSLang {
typedef PSLang::Parser::token token;
CodeGenLLVM::CodeGenLLVM(std::string fileName) :
		outputStream(fileName, std::ofstream::out), builder(getGlobalContext()), mainBlock(
				nullptr) {

	theModule = new Module("pslang module", getGlobalContext());

	theFPM = new FunctionPassManager(theModule);
	// Provide basic AliasAnalysis support for GVN.
	theFPM->add(createBasicAliasAnalysisPass());
	// Do simple "peephole" optimizations and bit-twiddling optzns.
	theFPM->add(createInstructionCombiningPass());
	// Reassociate expressions.
	theFPM->add(createReassociatePass());
	// Eliminate Common SubExpressions.
	theFPM->add(createGVNPass());
	// Simplify the control flow graph (deleting unreachable blocks, etc).
	theFPM->add(createCFGSimplificationPass());
//theFPM->add(createPrintModulePass(&outs()));
	theFPM->doInitialization();

}

CodeGenLLVM::~CodeGenLLVM() {
	// TODO Auto-generated destructor stub
}

static Type *typeOf(const NIdentifier& type) {
	if (type.name.compare("int") == 0) {
		return Type::getInt64Ty(getGlobalContext());
	} else if (type.name.compare("double") == 0) {
		return Type::getDoubleTy(getGlobalContext());
	}
	return Type::getVoidTy(getGlobalContext());
}

llvm::Value* CodeGenLLVM::getVariable(const std::string& name) {
	return namedValues[name];
}

void CodeGenLLVM::generateCode(NBlock& root) {
	std::cout << "Generating code...\n";

	/* Create the top level interpreter function to call as entry */
	vector<Type*> argTypes;
	FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()),
			argTypes, false);
	Function* mainFunction = Function::Create(ftype,
			GlobalValue::InternalLinkage, "main", theModule);
	mainBlock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction,
			0);

	root.codeGen(*this);

	ReturnInst::Create(getGlobalContext(), mainBlock);
	//raw_os_ostream rawStream(outputStream);
//	WriteBitcodeToFile(theModule,rawStream);
	theModule->dump();
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

	return new LoadInst(V, "", false, context.mainBlock);
}

Value* NMethodCall::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NBinaryOperator::codeGen(CodeGenLLVM& context) {
	std::cout << "Creating binary operation " << std::endl;
	Instruction::BinaryOps instr;
	switch (op) {
	case token::TPLUS:
		instr = Instruction::Add;
		break;
	case token::TMINUS:
		instr = Instruction::Sub;
		break;
	case token::TMUL:
		instr = Instruction::Mul;
		break;
	case token::TDIV:
		instr = Instruction::SDiv;
		break;
	default:
		return nullptr;
	}

	return BinaryOperator::Create(instr, lhs.codeGen(context),
			rhs.codeGen(context), "", context.mainBlock);
}

Value* NAssignment::codeGen(CodeGenLLVM& context) {
	std::cout << "Creating assignment for " << lhs.name << std::endl;
	if (context.namedValues.find(lhs.name) == context.namedValues.end()) {
		std::cerr << "undeclared variable " << lhs.name << std::endl;
		return NULL;
	}
	return new StoreInst(rhs.codeGen(context), context.namedValues[lhs.name],
			false, context.mainBlock);
}

Value* NBlock::codeGen(CodeGenLLVM& context) {

	StatementList::const_iterator it;
	Value *last = NULL;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cout << "Generating code for " << typeid(**it).name() << std::endl;
		last = (**it).codeGen(context);
	}
	std::cout << "Creating block" << std::endl;

	return last;
}

Value* NExpressionStatement::codeGen(CodeGenLLVM& context) {
	std::cout << "Generating code for " << typeid(expression).name()
			<< std::endl;
	return expression.codeGen(context);
}

Value* NArrayDeclaration::codeGen(CodeGenLLVM& context) {
	return (nullptr);
}

Value* NVariableDeclaration::codeGen(CodeGenLLVM& context) {
	std::cout << "Creating variable declaration " << type.name << " " << id.name
			<< std::endl;
	AllocaInst *alloc = new AllocaInst(typeOf(type), id.name.c_str(),
			context.mainBlock);
	context.namedValues[id.name] = alloc;
	if (assignmentExpression != NULL) {
		NAssignment assn(id, *assignmentExpression);
		assn.codeGen(context);
	}
	return alloc;
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
