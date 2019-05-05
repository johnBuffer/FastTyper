#pragma once

#include <SFML/Graphics.hpp>
#include "rectangle.hpp"
#include "letter.hpp"
#include "rounded_rectangle.hpp"
#include "line.hpp"
#include "wordinfo.hpp"
#include "cursor.hpp"

#include <iostream>

class TextDisplayer : public Rectangle, public sf::Drawable
{
public:
	TextDisplayer()
		: Rectangle()
		, m_space_y(0.0f)
		, m_char_size(0)
		, m_cursor(0.0f, 0.0f, 0.0f)
	{}

	TextDisplayer(float width, float height, float x, float y, uint32_t char_size)
		: Rectangle(width, height, x, y)
		, m_space_y(0.0f)
		, m_margin(50.0f)
		, m_text_start_y(0.0f)
		, m_char_size(char_size)
		, m_current_line(-1)
		, m_lines_to_display(2)
		, m_cursor(0.0f, y, 16.0f)
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

	void nextLine(uint32_t current_char, float word_width)
	{
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

		setCursorState(current_char, word_width);
	}

	Letter& getLetterAt(uint32_t i)
	{
		return m_letters[i];
	}

	std::vector<Letter>& getLetters()
	{
		return m_letters;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_cursor);

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

	void setCursorProgress(float percent)
	{
		m_cursor.setProgress(percent);
	}

	void setCursorState(uint32_t current_char, float word_width)
	{
		m_cursor.setState(m_letters[current_char].getX(), word_width);
	}

	void reset()
	{
		m_current_line = -1;
		m_letters.clear();
	}

	void initialize(const std::vector<WordInfo>& words)
	{
		const float space_x(16.0f);

		Line line(m_margin, 0.0f);
		line.pos.x = m_margin;
		line.pos.y = m_text_start_y;

		for (const WordInfo& word : words)
		{
			wordToLetters(line, word.string, m_text);
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

	std::vector<Letter> m_letters;

	void wordToLetters(Line& line, const std::string& word, const sf::Text& text)
	{
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
};
