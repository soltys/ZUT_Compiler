/*
 * Variable.h
 *
 *  Created on: Mar 11, 2013
 *      Author: soltys
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

namespace PSLang {
enum VariableType{
	Int,
	Float
};
class Variable {
public:
	Variable(const int memoryIndex, const VariableType type);
	virtual ~Variable();

	int memoryOffset;
	int size;
	VariableType type;
};

} /* namespace PSLang */
#endif /* VARIABLE_H_ */
