#pragma once
#include <vector>
#include <numeric>
#include <sstream>

template<typename T>
std::string numberToString(const T& n)
{
	std::stringstream sx;
	sx << n;
	return sx.str();
}


