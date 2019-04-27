#pragma once

#include <SFML/Graphics.hpp>

template<uint8_t N=0>
struct Theme
{
	static const sf::Color LetterOk;
	static const sf::Color LetterWrong;
	static const sf::Color LetterCorrectd;
	static const sf::Color LetterUnknown;
	static const sf::Color LetterSkipped;
	static const sf::Color Color1;
};

template<uint8_t N>
const sf::Color Theme<N>::LetterOk(146, 209, 139);

template<uint8_t N>
const sf::Color Theme<N>::LetterWrong(204, 104, 109);

template<uint8_t N>
const sf::Color Theme<N>::LetterCorrectd(212, 180, 116);

template<uint8_t N>
const sf::Color Theme<N>::LetterUnknown(200, 200, 200);

template<uint8_t N>
const sf::Color Theme<N>::LetterSkipped(64, 64, 64);

template<uint8_t N>
const sf::Color Theme<N>::Color1(36, 142, 230);

