#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "rectangle.hpp"
#include <iostream>

class Chart : public Rectangle, public sf::Drawable
{
public:

	Chart()
		: Rectangle(0.0f, 0.0f, 0.0f, 0.0f)
		, m_start(0.0f)
		, m_length(0.0f)
		, m_max(0.0f)
		, m_min(0.0f)
		, m_color(sf::Color::White)
	{}

	Chart(float width, float height, float x, float y)
		: Rectangle(width, height, x, y)
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

	float getFilteredValue(float in_value, uint32_t index) const
	{
		const uint32_t filter_width(9);
		const uint32_t mid_width((filter_width - 1) / 2);
		const uint32_t value_count(m_values.size());

		if (filter_width > value_count)
			return in_value;

		if (index < mid_width || index > value_count - mid_width - 1)
			return in_value;

		const float coefs[] = {-21.0f, 14.0f, 39.0f, 54.0f, 59.0f};
		const float norm(231.0f);

		float sum(0.0f);
		uint32_t start_i(index - mid_width);
		for (uint32_t i(0); i<mid_width; ++i)
		{
			const uint32_t index(i);
			sum += m_values[start_i + index] * coefs[i];

			const uint32_t r_index(mid_width - i + 1);
			sum += m_values[start_i + r_index] * coefs[i];
		}

		sum += m_values[start_i + mid_width] * coefs[mid_width];

		return sum / norm;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		const uint32_t size(m_values.size());
		sf::VertexArray va(sf::TriangleStrip, 2 * size);
		//sf::VertexArray va(sf::LinesStrip, size);

		uint32_t i(0);
		for (float v : m_values)
		{
			const float filtered = getFilteredValue(v, i);
			const float ratio(i / float(size));
			const float height_ratio(filtered / m_max);
			const float x(m_x + ratio * m_width);
			const float y(m_y + m_height - height_ratio * m_height);

			const sf::Color color(height_ratio*m_color.r, height_ratio*m_color.g, height_ratio*m_color.b);

			va[2*i + 0].position = sf::Vector2f(x, y);
			va[2*i + 1].position = sf::Vector2f(x, m_y + m_height);
			va[2*i + 0].color = m_color;
			va[2*i + 1].color = m_color;

			//va[i].position = sf::Vector2f(x, y);
			//va[i].color = m_color;

			++i;
		}

		target.draw(va, states);
	}

private:
	float m_start;
	float m_length;

	std::vector<float> m_values;
	float m_max;
	float m_min;

	sf::Color m_color;
};
