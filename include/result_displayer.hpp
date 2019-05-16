#pragma once
#include "rectangle.hpp"
#include <SFML/Graphics.hpp>

class ResultDisplayer : public Rectangle, public sf::Drawable
{
public:
	ResultDisplayer();
};
