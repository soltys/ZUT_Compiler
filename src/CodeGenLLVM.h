/*
 * CodeGenLLVM.h
 *
 *  Created on: Apr 22, 2013
 *      Author: soltys
 */

#ifndef CODEGENLLVM_H_
#define CODEGENLLVM_H_

#include "CodeGenBase.hpp"
#include <string>
#include <memory>
#include <stack>
#include <map>
#include <vector>
#include <fstream>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Type.h>
#include <llvm/DerivedTypes.h>
#include <llvm/LLVMContext.h>
#include <llvm/PassManager.h>
#include <llvm/Instructions.h>
#include <llvm/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Pass.h>
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/TargetSelect.h"
#include <llvm/Support/raw_os_ostream.h>
namespace PSLang {
class NBlock;
class CodeGenLLVM: public PSLang::CodeGenBase {

	std::ofstream outputStream;

	llvm::FunctionPassManager*  theFPM;



public:
	llvm::Module *theModule;
	llvm::IRBuilder<> builder;
	llvm::BasicBlock* mainBlock;
	std::map<std::string, llvm::Value*> namedValues;
	llvm::Value* getVariable(const std::string& name);
	CodeGenLLVM(std::string fileName);
	virtual void generateCode(NBlock& root);
	virtual ~CodeGenLLVM();

};

} /* namespace PSLang */
#endif /* CODEGENLLVM_H_ */
