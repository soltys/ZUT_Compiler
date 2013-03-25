/*
 * Utils.hpp
 *
 *  Created on: Mar 25, 2013
 *      Author: soltys
 */
#include"Instruction.h"
#include<iostream>
#include<sstream>
#ifndef UTILS_HPP_
#define UTILS_HPP_



namespace PSLang{
template<class T>
std::string toString(T a) {
	std::stringstream out;
	out << a;
	return out.str();
}



}

#endif /* UTILS_HPP_ */
