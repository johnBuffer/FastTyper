#include "challenge_words.hpp"

std::vector<std::string> ChallengeWords::s_words_set;

ChallengeWords::ChallengeWords(uint32_t width, uint32_t height) :
	m_width(width),
	m_height(height),
	m_current_word(0)
{
	m_font.loadFromFile("C:/Users/Jean/Documents/Code/cpp/FastTyper/font_med.ttf");
	initwords();
}

void ChallengeWords::render(sf::RenderTarget& target)
{
	for (const Letter& letter : m_letters)
	{
		if (letter.getLine() > 2)
			break;

		letter.draw(target);
	}
}

void ChallengeWords::init(const std::string & dico_path)
{
	s_words_set = { "lol", "yes", "ok", "parents", "house" };
}

void ChallengeWords::wordToLetters(Line& line, const std::string& word, const sf::Text& text)
{
	const float letter_space(2.0f);
	const uint32_t start_index(m_letters.size());
	uint32_t current_index(0);
	for (const char c : word)
	{
		m_letters.emplace_back(c, line.pos, line.line_count, text);
		Letter& current_letter(m_letters.back());
		line.pos.x += current_letter.getBounds().width + letter_space;

		// If we hit the max width
		if (line.pos.x > m_width)
		{
			// Increase y and reset x
			line.newLine();
			// Update letters
			for (uint32_t i(0); i < current_index + 1; ++i)
			{
				uint32_t letter_index(start_index + i);
				Letter& letter_to_update(m_letters[letter_index]);
				letter_to_update.setPosition(line.pos);
				letter_to_update.addLine();
				line.pos.x += m_letters[letter_index].getBounds().width + letter_space;
			}
		}

		++current_index;
	}
}

void ChallengeWords::initwords()
{
	sf::Text text;
	text.setFont(m_font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(42);
	text.setString("A");

	const float margin(25.0f);
	const float space_x(20.0f);

	Line line(margin, text.getGlobalBounds().height * 1.5f);

	const uint32_t word_count(300);
	uint32_t size(s_words_set.size());

	for (uint32_t i(word_count); --i;)
	{
		uint32_t index(rand() % size);
		wordToLetters(line, s_words_set[index], text);
		line.pos.x += space_x;
	}
}
