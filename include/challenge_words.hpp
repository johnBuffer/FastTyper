#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <transition.hpp>
#include "letter.hpp"
#include "line.hpp"
#include "wordinfo.hpp"
#include "cursor.hpp"

class ChallengeWords
{
public:
	ChallengeWords(uint32_t width, uint32_t height);

	void nextLine();
	void render(sf::RenderTarget& target);
	void addChar(char c);
	void removeChar();

	WordInfo& getCurrentWord()
	{
		return m_words[m_current_word];
	}

	const WordInfo& getCurrentWord() const
	{
		return m_words[m_current_word];
	}

	Letter& getLetter()
	{
		return m_letters[m_current_char];
	}

	float getProgress() const;

	static void init(const std::string& dico_path);

private:
	const uint32_t m_width;
	const uint32_t m_height;
	float          m_space_y;
	sf::Font       m_font;
	uint32_t       m_char_size;

	int32_t        m_current_line;
	uint32_t       m_current_char;
	uint32_t       m_current_word;
	std::string    m_typed;
	
	std::vector<WordInfo> m_words;
	std::vector<Letter>   m_letters;

	Cursor m_cursor;

	static std::vector<std::string> s_words_set;
	
	void wordToLetters(Line& line, const std::string& word, const sf::Text& text);
	void initwords(const sf::Text& text);
	uint32_t getCurrentCharInWord() const;
};
