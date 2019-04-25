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


template<typename T>
std::string toString(const T& number, uint32_t decimals)
{
	std::string result;
	std::stringstream sx;
	sx << std::fixed << number;
	result = sx.str();

	size_t pos = result.find('.');
	if (pos == std::string::npos)
		return result;
	else if (!decimals)
	{
		return result.substr(0, pos);
	}
	else
	{
		if (pos + decimals + 1 < result.size())
		{
			return result.substr(0, pos + decimals + 1);
		}
	}

	return result;
}