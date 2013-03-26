/*
 * Instruction.cpp
 *
 *  Created on: Mar 2, 2013
 *      Author: soltys
 */
#include <sstream>
#include "Instruction.h"

namespace PSLang {
int Instruction::_instuctionCounter = 0;

int Instruction::getInstuctionNumber()
{
	return _instuctionNumber;
}

 std::string Instruction::memoryParam(int memoryIndex )
{
	std::stringstream out;
	out << "#" << memoryIndex;
	return out.str();
}



} /* namespace PSLang */
