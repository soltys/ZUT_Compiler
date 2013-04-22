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
namespace PSLang {

class CodeGenLLVM: public PSLang::CodeGenBase {
public:
	CodeGenLLVM(std::string fileName);
	virtual void generateCode(NBlock& root);
	virtual ~CodeGenLLVM();
};

} /* namespace PSLang */
#endif /* CODEGENLLVM_H_ */
