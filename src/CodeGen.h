/*
 * CodeGen.h
 *
 *  Created on: Mar 2, 2013
 *      Author: soltys
 */

#ifndef CODEGEN_H_
#define CODEGEN_H_
#include "Node.h"
#include "Symbol.h"
#include "Instruction.h"
#include <fstream>
#include <stack>
#include <map>
#include <vector>
#include <memory>
namespace PSLang{
class NBlock;

class CodeGenContext {

	int _temporaryVariableCounter;
public:
	std::ofstream outputStream;
	CodeGenContext():_temporaryVariableCounter(0),outputStream("app.asm"){

	}
	PSLang::Variable createVariable(std::string& name,PSLang::SymbolType type,bool isTemporary = false);
	PSLang::Variable createTemporaryVariable(PSLang::SymbolType type);
	void generateCode(NBlock& root);
	std::string resultRegister;
    std::map<std::string, PSLang::Variable> locals;
    std::stack<std::shared_ptr<PSLang::Symbol>> valueStack;
    std::vector<PSLang::Instruction> programInstructions;

    virtual ~CodeGenContext(){

    }
};
}
#endif /* CODEGEN_H_ */
