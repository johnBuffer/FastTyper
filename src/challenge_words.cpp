#include "challenge_words.hpp"

#include <iostream>

std::vector<std::string> ChallengeWords::s_words_set;

ChallengeWords::ChallengeWords(uint32_t width, uint32_t height)
	: m_width(width)
	, m_height(height)
	, m_current_line(-1)
	, m_char_size(32)
	, m_current_char(0)
	, m_current_word(0)
	, m_typed("")
	, m_cursor(0.0f, 175.0f, 16.0f)
{
	m_font.loadFromFile("C:/Users/Jean/Documents/Code/cpp/FastTyper/play.ttf");
	m_cursor.setFont(m_font);
	
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
		const int32_t lines_to_display(2);

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
	target.draw(m_cursor);

	for (Letter& letter : m_letters)
	{
		target.draw(letter);
	}

	sf::Text text;
	text.setFont(m_font);
	text.setCharacterSize(42);
	text.setFillColor(sf::Color::White);
	text.setPosition(600, 600);
	text.setString(m_typed);
	target.draw(text);
}

void ChallengeWords::addChar(char c)
{
	if (c > 127 || c < 9)
	{
		return;
	}

	if (c == ' ')
	{
		const Letter& last_letter(getLetter());
		m_typed.clear();

		getCurrentWord().skipRest(m_letters);

		++m_current_word;
		m_current_char = getCurrentWord().start_index;

		// Cursor update
		m_cursor.setState(getLetter().getX(), getCurrentWord().getWordLength(m_letters));	

		if (getLetter().getLine() != last_letter.getLine())
			nextLine();
	}
	else
	{
		m_typed += c;
		bool ok(getLetter().check(c));
		if (m_typed.size() < getCurrentWord().length)
		{
			++m_current_char;
		}
	}

	m_cursor.setProgress(getProgress());
}

void ChallengeWords::removeChar()
{
	uint32_t size(m_typed.size());
	if (!size)
		return;

	--size;

	std::cout << size << " " << m_current_char << std::endl;
	if (size == getCurrentCharInWord())
	{
		getLetter().setState(Letter::Skipped);
		std::cout << "Removed " << getLetter().getChar() << std::endl;
		std::cout << "Current char " << getCurrentCharInWord() << std::endl;
	}
	else if (m_current_char && size < getCurrentCharInWord())
	{
		--m_current_char;
		getLetter().setState(Letter::Skipped);
	}

	m_typed = m_typed.substr(0, size);
	m_cursor.setProgress(getProgress());
}

float ChallengeWords::getProgress() const
{
	return 100.0f * (m_typed.size() / float(getCurrentWord().length));
}

void ChallengeWords::init(const std::string& dico_path)
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
		const std::string word(s_words_set[index]);
		m_words.emplace_back(word, m_letters.size());

		wordToLetters(line, word, text);
		line.pos.x += space_x;
	}
}

uint32_t ChallengeWords::getCurrentCharInWord() const
{
	return m_current_char - getCurrentWord().start_index;
}
