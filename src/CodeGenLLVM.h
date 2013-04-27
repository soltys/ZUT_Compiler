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
#include <stack>
#include <map>
#include <vector>
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

namespace PSLang {
class NBlock;
class CodeGenLLVM: public PSLang::CodeGenBase {

	llvm::Module *TheModule;

	std::map<std::string, llvm::Value*> namedValues;
public:
	llvm::IRBuilder<> builder;
	llvm::Value* getVariable(const std::string& name);
	CodeGenLLVM(std::string fileName);
	virtual void generateCode(NBlock& root);
	virtual ~CodeGenLLVM();

};

} /* namespace PSLang */
#endif /* CODEGENLLVM_H_ */
