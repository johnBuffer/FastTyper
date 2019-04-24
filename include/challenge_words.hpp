#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <transition.hpp>
#include "letter.hpp"
#include "line.hpp"

class ChallengeWords
{
public:
	ChallengeWords(uint32_t width, uint32_t height);

	void nextLine();
	void render(sf::RenderTarget& target);
	void addChar(char c);

	static void init(const std::string& dico_path);

private:
	const uint32_t m_width;
	const uint32_t m_height;
	uint32_t       m_current_word;
	float          m_space_y;
	sf::Font       m_font;
	int32_t        m_current_line;
	uint32_t       m_current_char;
	
	std::vector<std::string> m_words;
	std::vector<Letter>      m_letters;

	static std::vector<std::string> s_words_set;
	
	void wordToLetters(Line& line, const std::string& word, const sf::Text& text);
	void initwords(const sf::Text& text);
};
