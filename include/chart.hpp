#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Chart : public sf::Drawable
{
public:
	Chart(float width, float height, float x, float y)
		: m_width(width)
		, m_height(height)
		, m_x(x)
		, m_y(y)
		, m_start(0.0f)
		, m_length(0.0f)
		, m_max(0.0f)
		, m_min(0.0f)
		, m_color(sf::Color::White)
	{}

	void setColor(const sf::Color& color)
	{
		m_color = color;
	}

	void addValue(float val, float t)
	{
		if (m_values.empty()) {
			m_start = t;
			m_max = val;
			m_min = val;
		}
		else {
			m_length = t - m_start;

			m_max = val > m_max ? val : m_max;
			m_min = val < m_min ? val : m_min;
		}

		m_values.push_back(val);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		const uint32_t size(m_values.size());
		//sf::VertexArray va(sf::TriangleStrip, 2 * size);
		sf::VertexArray va(sf::LinesStrip, size);

		uint32_t i(0);
		for (float v : m_values)
		{
			const float ratio(i / float(size));
			const float x(m_x + ratio * m_width);
			const float y(m_y + m_height - (v / m_max) * m_height);

			/*va[2*i + 0].position = sf::Vector2f(x, y);
			va[2*i + 1].position = sf::Vector2f(x, m_y + m_height);
			va[2*i + 0].color = m_color;
			va[2*i + 1].color = sf::Color(0, 0, 0, 0);*/

			va[i].position = sf::Vector2f(x, y);
			va[i].color = m_color;

			++i;
		}

		target.draw(va, states);
	}

private:
	float m_width;
	float m_height;
	float m_x;
	float m_y;

	float m_start;
	float m_length;

	std::vector<float> m_values;
	float m_max;
	float m_min;

	sf::Color m_color;
};
