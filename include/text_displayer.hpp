#pragma once

#include <SFML/Graphics.hpp>
#include "rectangle.hpp"
#include "letter.hpp"
#include "rounded_rectangle.hpp"
#include "line.hpp"
#include "wordinfo.hpp"
#include "cursor.hpp"
#include "challenge_status.hpp"

#include <iostream>

class TextDisplayer : public Rectangle, public sf::Drawable
{
public:
	TextDisplayer(float width, float height, float x, float y, uint32_t char_size)
		: Rectangle(width, height, x, y)
		, m_space_y(0.0f)
		, m_margin(50.0f)
		, m_text_start_y(0.0f)
		, m_char_size(char_size)
		, m_current_line(-1)
		, m_lines_to_display(2)
		, m_cursor(0.0f, y, 16.0f)
		, m_current_word(0)
		, m_current_char(0)
	{}

	void setFont(const sf::Font& font)
	{
		m_font = font;
		m_space_y = m_font.getLineSpacing(m_char_size) + 8;
		m_cursor.setFont(font);

		m_text.setFont(font);
		m_text.setFillColor(Theme<>::LetterUnknown);
		m_text.setCharacterSize(m_char_size);

		m_text_start_y = m_y + (m_lines_to_display + 2) * m_space_y;
	}

	void nextLine()
	{
		if (m_current_line == -1) {
			m_cursor.setWordLenght(getCurrentWord().length);
			m_cursor.setState(getCurrentLetter().getX(), getCurrentWord().getWordWidth(m_letters));
		}

		m_current_line += 1;
		for (Letter& letter : m_letters)
		{
			int32_t line(letter.getLine());
			if (line == m_current_line - 1)
			{
				letter.setY(-100.0f);
			}
			else if (line >= m_current_line && line < (m_current_line + m_lines_to_display))
			{
				int32_t i(m_current_line - line);
				letter.setY(m_y - i * m_space_y);
			}
		}
	}

	Letter& getCurrentLetter()
	{
		const WordInfo& current_word(getCurrentWord());
		uint32_t max_index(current_word.start_index + current_word.length - 1);
		return m_letters[std::min(m_current_char, max_index)];
	}

	Letter& getLetterAt(uint32_t i)
	{
		return m_letters[i];
	}

	uint32_t getCurrentCharInWord() const
	{
		return m_current_char - getCurrentWord().start_index;
	}

	float getProgress() const
	{
		return 100.0f * (getCurrentCharInWord() / float(getCurrentWord().length));
	}

	uint32_t getCurrentCharIndex() const
	{
		return m_current_char;
	}

	std::vector<Letter>& getLetters()
	{
		return m_letters;
	}

	bool nextChar(char c, uint32_t typed_size)
	{
		const uint32_t max_length(getCurrentWord().length);
		if (typed_size > max_length) {
			return false;
		}
		else {
			Letter& currentLetter(getCurrentLetter());
			++m_current_char;
			m_cursor.addChar();
			return currentLetter.check(c);
		}
	}

	void prevChar()
	{
		--m_current_char;
		m_cursor.prevLetter();
		getCurrentLetter().setState(Letter::Skipped);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		if (m_current_line > -1) {
			target.draw(m_cursor);
		}

		RoundedRectangle text_zone(m_width - m_margin, m_space_y * m_lines_to_display, 12.0f, m_margin*0.5f, m_y);
		text_zone.setFillColor(Theme<>::Color2);
		target.draw(text_zone);

		for (const Letter& letter : m_letters) {
			if (letter.getY() > -m_space_y && letter.getY() < m_text_start_y) {
				target.draw(letter);
			}
		}
	}

	void drawBloom(sf::RenderTarget& target, sf::RenderStates states) const
	{
		m_cursor.drawProgress(target);

		RoundedRectangle text_zone(m_width - m_margin, m_space_y * m_lines_to_display, 12.0f, m_margin*0.5f, m_y);
		text_zone.setFillColor(sf::Color::Black);
		target.draw(text_zone);

		for (const Letter& letter : m_letters) {
			if (letter.getY() > -m_space_y && letter.getY() < m_text_start_y) {
				if (letter.getState() == Letter::Wrong) {
					target.draw(letter);
				}
			}
		}
	}

	WordInfo& getCurrentWord()
	{
		return m_words[m_current_word];
	}

	const WordInfo& getCurrentWord() const
	{
		return m_words[m_current_word];
	}

	uint32_t nextWord()
	{
		const uint32_t skipped(getCurrentWord().skipRest(m_letters));
		++m_current_word;
		m_current_char = getCurrentWord().start_index;
		if (getCurrentWord().first_of_line) {
			nextLine();
		}
		
		m_cursor.setWordLenght(getCurrentWord().length);
		m_cursor.setState(getCurrentLetter().getX(), getCurrentWord().getWordWidth(m_letters));
		
		return skipped;
	}

	void reset()
	{
		m_current_line = -1;
		m_current_char = 0;
		m_current_word = 0;
		m_letters.clear();
		m_words.clear();
	}

	void initialize(std::vector<std::string>& words)
	{
		const float space_x(16.0f);

		Line line(m_margin, 0.0f);
		line.pos.x = m_margin;
		line.pos.y = m_text_start_y;

		for (const std::string& word : words)
		{
			m_words.emplace_back(word, m_letters.size());
			m_words.back().first_of_line = wordToLetters(line, word, m_text);
			line.pos.x += space_x;
		}
	}

private:
	int32_t m_current_line;

	float m_space_y;
	float m_margin;
	float m_text_start_y;

	sf::Font m_font;
	sf::Text m_text;
	uint32_t m_char_size;
	uint32_t m_lines_to_display;

	Cursor m_cursor;
	uint32_t m_current_word;
	uint32_t m_current_char;

	std::vector<WordInfo> m_words;
	std::vector<Letter>   m_letters;

	bool wordToLetters(Line& line, const std::string& word, const sf::Text& text)
	{
		bool new_line(false);
		const uint32_t start_index(m_letters.size());
		uint32_t current_index(0);
		for (const char c : word)
		{
			m_letters.emplace_back(c, line.pos, line.line_count, text);
			Letter& current_letter(m_letters.back());
			line.pos.x += current_letter.getAdvance();

			// If we hit the max width
			if (line.pos.x > m_width - line.margin)
			{
				// New line
				new_line = true;
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

		return new_line;
	}
};
