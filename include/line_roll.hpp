#include "letter.hpp"
#include "rectangle.hpp"
#include "font_dependant.hpp"
#include "line.hpp"
#include "wordinfo.hpp"


class LineRoll : public sf::Drawable, public Rectangle, public FontDependant
{
public:
	LineRoll(float width, float x, float y, uint32_t line_to_display)
		: Rectangle(width, 0.0f, x, y)
		, FontDependant()
		, m_line_count(line_to_display)
		, m_current_line(-1)
		, m_space_y(0.0f)
	{}

	void roll()
	{
		m_current_line += 1;
		for (Letter& letter : m_letters) {
			int32_t line(letter.getLine());
			if (line == m_current_line - 1) {
				letter.setY(-100.0f);
			}
			else if (line >= m_current_line && line < (m_current_line + m_line_count)) {
				int32_t i(m_current_line - line);
				letter.setY(m_y - i * m_space_y);
			}
		}
	}

	void setFont(const sf::Font& font, uint32_t char_size) override
	{
		FontDependant::setFont(font, char_size);

		const float line_spacing_margin(8.0f);
		m_space_y = font.getLineSpacing(char_size) + line_spacing_margin;
		m_height = m_space_y * m_line_count;
		m_text_start_y = m_y + (m_line_count + 2) * m_space_y;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		const float bottom_disp(m_y + m_height);
		const float bottom_text(m_text_start_y + m_space_y);
		const float mid((bottom_disp + bottom_text) * 0.5f);

		for (const Letter& letter : m_letters) {
			if (letter.getY() > m_y * 0.5f && letter.getY() < mid - m_space_y) {
				target.draw(letter);
			}
		}

		sf::VertexArray va_down(sf::Quads, 4);
		va_down[0].position = sf::Vector2f(0.0f, bottom_disp);
		va_down[1].position = sf::Vector2f(m_width, bottom_disp);
		va_down[2].position = sf::Vector2f(m_width, mid);
		va_down[3].position = sf::Vector2f(0.0f, mid);

		va_down[0].color = m_background_color_transp;
		va_down[1].color = m_background_color_transp;
		va_down[2].color = m_background_color;
		va_down[3].color = m_background_color;

		target.draw(va_down);

		sf::VertexArray va_up(sf::Quads, 4);
		va_up[0].position = sf::Vector2f(0.0f, m_y);
		va_up[1].position = sf::Vector2f(m_width, m_y);
		va_up[2].position = sf::Vector2f(m_width, m_y*0.5f);
		va_up[3].position = sf::Vector2f(0.0f, m_y*0.5f);

		va_up[0].color = m_background_color_transp;
		va_up[1].color = m_background_color_transp;
		va_up[2].color = m_background_color;
		va_up[3].color = m_background_color;

		target.draw(va_up);
	}

	void drawBloom(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (const Letter& letter : m_letters) {
			if (letter.getY() > -m_space_y && letter.getY() < m_text_start_y) {
				if (letter.getState() == Letter::Wrong) {
					target.draw(letter);
				}
			}
		}
	}

	void generateLetters(float margin, const std::vector<std::string>& words, std::vector<WordInfo>& wordinfo)
	{
		const float space_x(16.0f);

		Line line(margin, 0.0f);
		line.pos.x = margin;
		line.pos.y = m_text_start_y;

		for (const std::string& word : words)
		{
			wordinfo.emplace_back(word, m_letters.size());
			wordToLetters(line, word, wordinfo.back());
			line.pos.x += space_x;
		}
	}

	void setBackgroundColor(const sf::Color& color)
	{
		m_background_color = color;
		m_background_color_transp = color;
		m_background_color_transp.a = 0.0f;
	}

	void clear()
	{
		m_letters.clear();
		m_current_line = -1;
	}

	Letter& getLetterAt(const uint32_t i)
	{
		return m_letters[i];
	}

	std::vector<Letter>& getLetters()
	{
		return m_letters;
	}

	int32_t getCurrentLine() const
	{
		return m_current_line;
	}

private:
	uint32_t m_line_count;
	int32_t  m_current_line;
	float    m_text_start_y;
	float    m_text_out_y;
	float    m_space_y;

	sf::Color m_background_color;
	sf::Color m_background_color_transp;

	std::vector<Letter> m_letters;

	void wordToLetters(Line& line, const std::string& word, WordInfo& wordinfo)
	{
		bool new_line(false);
		const uint32_t start_index(m_letters.size());
		uint32_t current_index(0);
		float width(0.0f);

		for (const char c : word)
		{
			m_letters.emplace_back(c, line.pos, line.line_count, getFont(), m_char_size);
			Letter& current_letter(m_letters.back());

			const float advance(current_letter.getAdvance());
			line.pos.x += advance;
			width += advance;

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

		const float min_width(65.0f);
		if (width < min_width)
		{
			line.pos.x += min_width - width;
		}

		wordinfo.width = width;
		wordinfo.first_of_line = new_line;
	}
};


