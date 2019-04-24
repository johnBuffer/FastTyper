#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "letter.hpp"
#include "line.hpp"

class ChallengeWords
{
public:
	ChallengeWords(uint32_t width, uint32_t height);

	void render(sf::RenderTarget& target);

	static void init(const std::string& dico_path);

private:
	const uint32_t m_width;
	const uint32_t m_height;
	uint32_t       m_current_word;
	sf::Font       m_font;
	
	std::vector<std::string> m_words;
	std::vector<Letter> m_letters;

	static std::vector<std::string> s_words_set;
	
	void wordToLetters(Line& line, const std::string& word, const sf::Text& text);
	void initwords();
};
