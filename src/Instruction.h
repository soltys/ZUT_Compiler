/*
 * Instruction.h
 *
 *  Created on: Mar 2, 2013
 *      Author: soltys
 */

#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_
#include <string>
#include <boost/algorithm/string.hpp>
namespace PSLang {



class Instruction {

	int _instuctionNumber;
	std::string command;
	std::string param1;
	std::string param2;
	friend std::ostream& operator <<(std::ostream& o, const Instruction& a);
public:
	static int _instuctionCounter;

	int getInstuctionNumber();
	static std::string memoryParam(int memoryIndex );
	Instruction(const std::string& command, const std::string& param1) :
			_instuctionNumber(Instruction::_instuctionCounter++),
			command(boost::to_upper_copy(command)), param1(param1)	 {
	}

	Instruction(const std::string& command, const std::string& param1, const std::string& param2) :
			_instuctionNumber(Instruction::_instuctionCounter++),
			command(boost::to_upper_copy(command)), param1(param1), param2(param2)
			 {
	}

	const std::string& getParam1() const {
		return param1;
	}

	void setParam1(const std::string& param1) {
		this->param1 = param1;
	}

	const std::string& getParam2() const {
		return param2;
	}

	void setParam2(const std::string& param2) {
		this->param2 = param2;
	}
};

} /* namespace PSLang */
#endif /* INSTRUCTION_H_ */
