/*
 * CodeGen.h
 *
 *  Created on: Mar 2, 2013
 *      Author: soltys
 */

#ifndef CODEGEN_H_
#define CODEGEN_H_
#include <fstream>
#include <stack>
#include <map>
namespace PSLang{
class NBlock;

class CodeGenContext {

public:
	std::ofstream outputStream;
	CodeGenContext():outputStream("app.asm"){
	}

	std::string resultRegister;
	void  generateCode(NBlock& root);
	void foo(){}
    std::map<std::string, int> locals;
};
}
#endif /* CODEGEN_H_ */
