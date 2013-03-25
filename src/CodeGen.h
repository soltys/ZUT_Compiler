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
namespace PSLang{
class NBlock;

class CodeGenContext {

public:
	std::ofstream outputStream;
	CodeGenContext():outputStream("app.asm"){

	}

	std::string resultRegister;
	void generateCode(NBlock& root);
    std::map<std::string, PSLang::Variable> locals;
    std::stack<double> valueStack;
    std::vector<PSLang::Instruction> programInstructions;

};
}
#endif /* CODEGEN_H_ */
