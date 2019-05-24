#pragma once

#include <map>
#include <string>
#include <fstream>

struct WordRatio
{
	WordRatio()
		: correct(0)
		, wrong(0)
	{}

	WordRatio(uint32_t c, uint32_t w)
		: correct(c)
		, wrong(w)
	{}

	uint32_t correct;
	uint32_t wrong;
};

class WordStats
{
public:
	WordStats()
	{
		loadFromFile();
	}

	void loadFromFile()
	{
		std::ifstream in_file;
		in_file.open("stats");

		if (in_file)
		{
			std::string word;
			uint32_t correct;
			uint32_t wrong;

			while (in_file >> word >> correct >> wrong) {
				addWordStat(word, correct, wrong);
			}
		}
	}

	void addWordStat(const std::string& word, uint32_t correct, uint32_t wrong)
	{
		m_stats[word] = WordRatio(correct, wrong);
	}

	void addStat(const std::string& str, bool correct)
	{
		if (correct) {
			m_stats[str].correct += 1;
		} else {
			m_stats[str].wrong += 1;
		}
	}

	void toFile()
	{
		std::ofstream out_file;
		out_file.open("stats");

		for (auto& p : m_stats) {
			out_file << p.first << " " << p.second.correct << " " << p.second.wrong << std::endl;
		}

		out_file.close();
	}

private:
	std::map<std::string, WordRatio> m_stats;
};


