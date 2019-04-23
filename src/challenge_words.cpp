#include "challenge_words.hpp"

std::vector<std::string> ChallengeWords::s_words_set;

ChallengeWords::ChallengeWords(uint32_t width, uint32_t height) :
	m_width(width),
	m_height(height)
{
	m_font.loadFromFile("C:/Users/Jean/Documents/Code/cpp/FastTyper/font_med.ttf");
	initwords();
}

void ChallengeWords::render(sf::RenderTarget& target)
{
	sf::Text text;
	text.setFont(m_font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(42);

	text.setString("A");

	const float margin(25.0f);
	const float space_x(20.0f);
	const float space_y(text.getGlobalBounds().height * 1.5f);

	float x(margin);
	uint32_t line_count(0);

	for (const std::string& word : m_words)
	{
		text.setString(word);

		const auto bounds(text.getGlobalBounds());
		if (x + bounds.width > m_width - margin)
		{
			x = margin;
			++line_count;
		}

		if (line_count == 2)
			break;

		text.setPosition(x, line_count * space_y + margin);
		target.draw(text);

		x += text.getGlobalBounds().width + space_x;
	}
}

void ChallengeWords::init(const std::string & dico_path)
{
	s_words_set = { "lol", "yes", "ok", "parents", "house" };
}

void ChallengeWords::initwords()
{
	const uint32_t word_count(300);
	uint32_t size(s_words_set.size());

	for (uint32_t i(word_count); --i;)
	{
		uint32_t index(rand() % size);
		m_words.push_back(s_words_set[index]);
	}
}
