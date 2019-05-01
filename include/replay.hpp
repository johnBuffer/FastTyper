#pragma once

#include <string>
#include <vector>
#include <sstream>
#include<fstream>

struct ReplayAction
{
	enum ActionType
	{
		None,
		AddChar,
		RemoveChar
	};

	ReplayAction()
		: action(ActionType::None)
		, char_code(0)
		, timestamp(0)
	{}

	ReplayAction(ActionType action_type, int32_t code, uint32_t ms_timestamp)
		: action(action_type)
		, char_code(code)
		, timestamp(ms_timestamp)
	{}

	const std::string toString() const
	{
		std::stringstream sx;
		sx << timestamp << " ";

		if (action == ActionType::AddChar)
		{
			sx << "ADD " << char_code;
		}
		else if (action == ActionType::RemoveChar)
		{
			sx << "REM";
		}

		return sx.str();
	}

	ActionType action;
	int32_t char_code;
	uint32_t timestamp;
};

class Replay
{
public:
	Replay() = default;

	void clear()
	{
		m_words.clear();
		m_actions.clear();
	}

	void addWord(const std::string& word)
	{
		m_words.push_back(word);
	}

	void addChar(int32_t code, uint32_t timestamp)
	{
		m_actions.emplace_back(ReplayAction::ActionType::AddChar, code, timestamp);
	}

	void removeChar(uint32_t timestamp)
	{
		m_actions.emplace_back(ReplayAction::ActionType::RemoveChar, 0, timestamp);
	}

	void toFile() const
	{
		std::ofstream file;
		file.open("example.txt");

		for (const std::string& word : m_words)
		{
			file << word << " ";
		}
		file << std::endl;

		for (const ReplayAction& ra : m_actions)
		{
			file << ra.toString() << std::endl;
		}
		file.close();
	}

	void loadFromFile()
	{
		std::ifstream file;
		file.open("example.txt");

		std::string line;
		std::getline(file, line);
		loadWords(line);

		uint32_t timestamp;
		std::string action_str;


		while (std::getline(file, line))
		{

		}
	}

private:
	std::vector<std::string> m_words;
	std::vector<ReplayAction> m_actions;

	void loadWords(const std::string& line)
	{
		const char sep(' ');
		uint32_t last_index(0);
		uint32_t index(line.find(sep));
		while (index != std::string::npos)
		{
			const uint32_t word_length(index - last_index);
			if (word_length)
			{
				const std::string word(line.substr(last_index, word_length));
				m_words.push_back(word);
			}
			last_index = index + 1;
		}
	}
};

