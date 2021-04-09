#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace Maths 
{
	static void NormalizeVector(sf::Vector2f& vector) 
	{
		float magnitude = std::sqrt(std::pow(vector.x, 2.f) + std::pow(vector.y, 2.f));
		vector = vector / magnitude;
	}

	static const sf::Vector2f RoundVector(const sf::Vector2f& vector) 
	{
		return { std::roundf(vector.x), std::roundf(vector.y) };
	}

	static const float GetDistance(const sf::Vector2f& vector1, const sf::Vector2f& vector2) 
	{
		return std::sqrt(std::pow(vector1.x - vector2.x, 2.f) + std::pow(vector1.y - vector2.y, 2.f));
	}
}