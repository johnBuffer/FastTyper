#pragma once

class Rectangle
{
public:
	Rectangle()
		: m_width(0.0f)
		, m_height(0.0f)
		, m_x(0.0f)
		, m_y(0.0f)
	{}

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

	void setSize(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	void setPosition(float x, float y)
	{
		m_x = x;
		m_y = y;
	}


protected:
	float m_width, m_height, m_x, m_y;
};

