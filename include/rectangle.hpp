#pragma once

class Rectangle
{
public:
	Rectangle(float width, float height, float x, float y)
		: m_width(width)
		, m_height(height)
		, m_x(x)
		, m_y(y)
	{}

	float getX() const
	{
		return m_x;
	}

	float getY() const
	{
		return m_y;
	}

protected:
	float m_width, m_height, m_x, m_y;
};

