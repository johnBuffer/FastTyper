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
		NextWord,
		RemoveChar,
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
		else if (action == ActionType::NextWord)
		{
			sx << "NXT " << 0;
		}
		else if (action == ActionType::RemoveChar)
		{
			sx << "REM " << 0;
		}

		return sx.str();
	}

	ActionType action;
	int32_t char_code;
	uint32_t timestamp;
};

class ChallengeRecorder
{
public:
	ChallengeRecorder()
		: m_exported(false)
	{

	}

	void clear()
	{
		m_words.clear();
		m_actions.clear();
		m_exported = false;
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

	void nextWord(uint32_t timestamp)
	{
		m_actions.emplace_back(ReplayAction::ActionType::NextWord, 0, timestamp);
	}

	bool isExported() const
	{
		return m_exported;
	}

	void toFile() const
	{
		uint32_t i(0);
		std::string filename("replay_0.rpl");
		while (exists(filename))
		{
			filename = "replay_" + toString(++i, 0) + ".rpl";
		}

		std::ofstream file;
		file.open(filename);

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

		m_exported = true;
	}

	void loadFromFile(const std::string& filename)
	{
		std::ifstream file;
		file.open(filename);

		if (!file)
		{
			std::cout << "Invalid replay file" << std::endl;
			return;
		}

		m_words.clear();

		std::string line;
		std::getline(file, line);
		loadWords(line);

		uint32_t timestamp;
		std::string action_str;
		uint32_t unicode;
		while (file >> timestamp >> action_str >> unicode)
		{
			std::cout << "TS " << timestamp << " Action " << action_str << std::endl;
			if (action_str == "ADD")
				m_actions.emplace_back(ReplayAction::ActionType::AddChar, unicode, timestamp);
			else if (action_str == "NXT")
				m_actions.emplace_back(ReplayAction::ActionType::NextWord, unicode, timestamp);
			else if (action_str == "REM")
				m_actions.emplace_back(ReplayAction::ActionType::RemoveChar, unicode, timestamp);
		}
	}

	const ReplayAction& getAction(uint32_t i) const
	{
		return m_actions[i];
	}

	std::size_t actionCount() const
	{
		return m_actions.size();
	}

	const std::vector<std::string>& getWords() const
	{
		return m_words;
	}

	bool isEmpty() const
	{
		return m_actions.empty();
	}

private:
	std::vector<std::string> m_words;
	std::vector<ReplayAction> m_actions;
	mutable bool m_exported;

	void loadWords(const std::string& line)
	{
		const char sep(' ');
		std::size_t last_index(0);
		std::size_t index(line.find(sep));
		while (index != std::string::npos)
		{
			const std::size_t word_length(index - last_index);
			if (word_length)
			{
				const std::string word(line.substr(last_index, word_length));
				std::cout << "Found '" << word << "'" << std::endl;
				m_words.push_back(word);
			}
			last_index = index + 1;
			index = line.find(sep, last_index);
		}
	}
};

