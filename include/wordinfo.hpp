#pragma once
#include <stdint.h>
#include <string>

struct WordInfo
{
	WordInfo()
		: string("")
		, start_index(0)
		, length(0)
	{}

	WordInfo(const std::string& str, uint32_t index)
		: string(str)
		, start_index(index)
		, length(str.size())
	{}

	const std::string string;
	uint32_t start_index;
	uint32_t length;
};