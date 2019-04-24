#include "challenge_words.hpp"

#include <iostream>

std::vector<std::string> ChallengeWords::s_words_set;

ChallengeWords::ChallengeWords(uint32_t width, uint32_t height)
	: m_width(width)
	, m_height(height)
	, m_current_word(0)
	, m_current_line(-1)
	, m_current_char(0)
	, m_char_size(32)
{
	m_font.loadFromFile("C:/Users/Jean/Documents/Code/cpp/FastTyper/play.ttf");
	
	sf::Text text;
	text.setFont(m_font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(m_char_size);
	m_space_y = m_font.getLineSpacing(m_char_size) + 8;

	initwords(text);
	nextLine();
}

void ChallengeWords::nextLine()
{
	for (Letter& letter : m_letters)
	{
		const int32_t lines_to_display(3);

		int32_t line(letter.getLine());
		if (line == m_current_line)
		{
			letter.setY(-100.0f);
		}
		else if (line == m_current_line + lines_to_display)
		{
			letter.setY(200.0f);
		}
		else if (line >= m_current_line && line < (m_current_line + lines_to_display))
		{
			int32_t i(m_current_line + lines_to_display - line);
			letter.setY(200.0f - i*m_space_y);
		}
	}

	m_current_line += 1;

}

void ChallengeWords::render(sf::RenderTarget& target)
{
	for (Letter& letter : m_letters)
	{
		target.draw(letter);
	}
}

void ChallengeWords::addChar(char c)
{
	if (c == ' ')
	{

	}
	else
	{
		Letter& current_letter(m_letters[m_current_char]);
		bool ok(current_letter.check(c));
		if (ok)
		{
			++m_current_char;
			if (m_letters[m_current_char].getLine() != current_letter.getLine())
			{
				nextLine();
			}
		}
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
		line.pos.x += current_letter.getAdvance();

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
				letter_to_update.setX(line.pos.x);
				letter_to_update.addLine();
				line.pos.x += letter_to_update.getAdvance();
			}
		}

		++current_index;
	}
}

void ChallengeWords::initwords(const sf::Text& text)
{
	const float margin(25.0f);
	const float space_x(20.0f);

	Line line(margin, 0.0f);
	line.pos.y = 900.0f;

	const uint32_t word_count(300);
	uint32_t size(s_words_set.size());

	for (uint32_t i(word_count); --i;)
	{
		uint32_t index(rand() % size);
		wordToLetters(line, s_words_set[index], text);
		line.pos.x += space_x;
	}
}
