/*
 * Expression.h
 *
 *  Created on: Feb 28, 2013
 *      Author: soltys
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

namespace PSLang {

class Expression {
public:
	Expression();
	virtual ~Expression();
private:
	Expression* _subExpression;
};

} /* namespace PSLang */
#endif /* EXPRESSION_H_ */
