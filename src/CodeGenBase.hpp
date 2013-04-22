/*
 * CodeGenContextBase.hpp
 *
 *  Created on: Apr 22, 2013
 *      Author: soltys
 */

#ifndef CODEGENCONTEXTBASE_H_
#define CODEGENCONTEXTBASE_H_

namespace PSLang {
class NBlock;
class CodeGenBase {
public:
	CodeGenBase(){};

	virtual void generateCode(NBlock &root) = 0;
	virtual ~CodeGenBase(){};
};

} /* namespace PSLang */
#endif /* CODEGENCONTEXTBASE_H_ */
