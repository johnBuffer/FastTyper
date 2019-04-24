#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class ChallengeWords
{
public:
	ChallengeWords(uint32_t width, uint32_t height);

	void render(sf::RenderTarget& target);

	static void init(const std::string& dico_path);

private:
	const uint32_t m_width;
	const uint32_t m_height;
	uint32_t m_current_word;
	
	std::vector<std::string> m_words;

	static std::vector<std::string> s_words_set;

	sf::Font m_font;

	void initwords();
};
