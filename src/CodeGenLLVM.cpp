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
namespace PSLang
{
typedef PSLang::Parser::token token;
CodeGenLLVM::CodeGenLLVM(std::string fileName) :
		outputStream(fileName, std::ofstream::out), builder(getGlobalContext()), mainBlock(nullptr)
{

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

CodeGenLLVM::~CodeGenLLVM()
{
	// TODO Auto-generated destructor stub
}

static Type *typeOf(const NIdentifier& type)
{
	if (type.name.compare("int") == 0)
	{
		return Type::getInt64Ty(getGlobalContext());
	}
	else if (type.name.compare("double") == 0)
	{
		return Type::getDoubleTy(getGlobalContext());
	}
	return Type::getVoidTy(getGlobalContext());
}

llvm::Value* CodeGenLLVM::getVariable(const std::string& name)
{
	return namedValues[name];
}

void CodeGenLLVM::generateCode(NBlock& root)
{
	std::cout << "Generating code...\n";

	/* Create the top level interpreter function to call as entry */
	vector<Type*> argTypes;
	FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()), argTypes, false);
	mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", theModule);
	mainBlock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);
	builder.SetInsertPoint(mainBlock);
	root.codeGen(*this);

	builder.CreateRet(ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 0, true));
	//theFPM->run(*mainFunction);
	//raw_os_ostream rawStream(outputStream);
//	WriteBitcodeToFile(theModule,rawStream);
	theModule->dump();
	std::cout << "Code is generated.\n";

}

Value* NInteger::codeGen(CodeGenLLVM& context)
{
	return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value, true);
}

Value* NDouble::codeGen(CodeGenLLVM& context)
{
	return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), value);
}
Value* NArrayIdentifier::codeGen(CodeGenLLVM& context)
{
	return (nullptr);
}
Value* NIdentifier::codeGen(CodeGenLLVM& context)
{
	Value *V = context.getVariable(name);

	return new LoadInst(V, "", false, context.mainBlock);
}

Value* NMethodCall::codeGen(CodeGenLLVM& context)
{
	return (nullptr);
}

Value* NBinaryOperator::codeGen(CodeGenLLVM& context)
{
	std::cout << "Creating binary operation " << std::endl;
	switch (op)
	{
	case token::TPLUS:
		return context.builder.CreateAdd(lhs.codeGen(context), rhs.codeGen(context));
		break;
	case token::TMINUS:
		return context.builder.CreateSub(lhs.codeGen(context), rhs.codeGen(context));
		break;
	case token::TMUL:
		return context.builder.CreateMul(lhs.codeGen(context), rhs.codeGen(context));
		break;
	case token::TDIV:
		return context.builder.CreateSDiv(lhs.codeGen(context), rhs.codeGen(context));
		break;
	case token::TCEQ:
		return context.builder.CreateICmpEQ(lhs.codeGen(context), rhs.codeGen(context));
	case token::TCNE:
		return context.builder.CreateICmpNE(lhs.codeGen(context), rhs.codeGen(context));
	case token::TCGT:
		return context.builder.CreateICmpUGT(lhs.codeGen(context), rhs.codeGen(context));
	case token::TCGE:
		return context.builder.CreateICmpUGE(lhs.codeGen(context), rhs.codeGen(context));
	case token::TCLT:
		return context.builder.CreateICmpULT(lhs.codeGen(context), rhs.codeGen(context));
	case token::TCLE:
		return context.builder.CreateICmpULE(lhs.codeGen(context), rhs.codeGen(context));

	default:
		return nullptr;
	}
	return nullptr;

}

Value* NAssignment::codeGen(CodeGenLLVM& context)
{
	std::cout << "Creating assignment for " << lhs.name << std::endl;
	if (context.namedValues.find(lhs.name) == context.namedValues.end())
	{
		std::cerr << "undeclared variable " << lhs.name << std::endl;
		return NULL;
	}
	return context.builder.CreateStore(rhs.codeGen(context), context.namedValues[lhs.name], false);

}

Value* NBlock::codeGen(CodeGenLLVM& context)
{

	StatementList::const_iterator it;
	Value *last = NULL;
	for (it = statements.begin(); it != statements.end(); it++)
	{
		std::cout << "Generating code for " << typeid(**it).name() << std::endl;
		last = (**it).codeGen(context);
	}
	std::cout << "Creating block" << std::endl;

	return last;
}

Value* NExpressionStatement::codeGen(CodeGenLLVM& context)
{
	std::cout << "Generating code for " << typeid(expression).name() << std::endl;
	return expression.codeGen(context);
}

Value* NArrayDeclaration::codeGen(CodeGenLLVM& context)
{
	return (nullptr);
}

Value* NVariableDeclaration::codeGen(CodeGenLLVM& context)
{
	std::cout << "Creating variable declaration " << type.name << " " << id.name << std::endl;
	AllocaInst *alloc = new AllocaInst(typeOf(type), id.name.c_str(), context.mainBlock);
	context.namedValues[id.name] = alloc;
	if (assignmentExpression != NULL)
	{
		NAssignment assn(id, *assignmentExpression);
		assn.codeGen(context);
	}
	return alloc;
}

Value* NFunctionDeclaration::codeGen(CodeGenLLVM& context)
{
	return (nullptr);
}

Value* NWhileStatement::codeGen(CodeGenLLVM& context)
{
	return (nullptr);
}
Value* NForStatement::codeGen(CodeGenLLVM& context)
{
	return (nullptr);
}

Value* NIfStatement::codeGen(CodeGenLLVM& context)
{
	Value* cond = boolExpr.codeGen(context);
	if (cond == nullptr)
	{
		return nullptr;
	}

	cond = context.builder.CreateFCmpONE(cond, ConstantFP::get(getGlobalContext(), APFloat(0.0)), "ifcond");

	BasicBlock *thenBB = BasicBlock::Create(getGlobalContext(), "then", context.mainFunction);

	BasicBlock *mergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");
	context.builder.CreateCondBr(cond, thenBB, mergeBB);
	context.builder.SetInsertPoint(thenBB);
	block.codeGen(context);
	context.builder.CreateBr(mergeBB);
	thenBB = context.builder.GetInsertBlock();
	context.mainFunction->getBasicBlockList().push_back(mergeBB);
	context.builder.SetInsertPoint(mergeBB);

	return mergeBB;
}

Value* NIfElseStatement::codeGen(CodeGenLLVM& context)
{
	return (nullptr);
}

Value* NBooleanOperator::codeGen(CodeGenLLVM& context)
{
	return (nullptr);
}
} /* namespace PSLang */
