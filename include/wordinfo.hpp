#pragma once
#include <stdint.h>
#include <string>
#include "letter.hpp"
#include <iostream>

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
		, length(string.size())
	{}

	float getWordLength(const std::vector<Letter>& letters)
	{
		const Letter& first(letters[start_index]);
		const Letter& last(letters[start_index+length-1]);

		return last.getX() + last.getAdvance() - first.getX();
	}

	void skipRest(std::vector<Letter>& letters)
	{
		for (uint32_t i(0); i < length; ++i)
		{
			if (letters[start_index + i].getState() == Letter::Unknown)
				letters[start_index + i].setState(Letter::Skipped);
		}
	}

	const std::string string;
	uint32_t start_index;
	uint32_t length;
};