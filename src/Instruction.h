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

public:
	static int _instuctionCounter;
	std::string instruction;
	int getInstuctionNumber();
	static std::string memoryParam(int memoryIndex );
	Instruction(const std::string& command, const std::string& param1) :
			_instuctionNumber(Instruction::_instuctionCounter++),
			instruction(boost::to_upper_copy(command) + std::string(" ") + param1) {
	}

	Instruction(const std::string& command, const std::string& param1, const std::string& param2) :
			_instuctionNumber(Instruction::_instuctionCounter++),
			instruction(boost::to_upper_copy(command) + std::string(" ") + param1 + std::string(",") + param2)
			 {

	}
};

} /* namespace PSLang */
#endif /* INSTRUCTION_H_ */
