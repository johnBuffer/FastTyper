#pragma once
#include <stdint.h>
#include <SFML/System/Vector2.hpp>

struct Line
{
	Line()
		: margin(0.0f)
		, pos(0.0f ,0.0f)
		, space_y(0.0f)
		, line_count(0)
	{}

	explicit Line(float margin_, float vertical_space)
		: margin(margin_)
		, pos(margin, margin)
		, space_y(vertical_space)
		, line_count(0)
	{}

	void newLine()
	{
		pos.x = margin;
		++line_count;
	}

	float margin;
	sf::Vector2f pos;
	float space_y;
	int32_t line_count;
};
