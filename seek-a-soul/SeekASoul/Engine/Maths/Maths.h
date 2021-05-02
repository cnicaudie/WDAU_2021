#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <random>

namespace SeekASoul
{
	namespace Engine
	{
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

			static const uint64_t GetDifference(const uint64_t first, const uint64_t second) 
			{
				uint64_t absoluteDifference = (first > second) ? (first - second) : (second - first);
				return (absoluteDifference <= INT64_MAX) ? absoluteDifference : INT64_MAX;
			}

			static const float GetRandom(const float minValue, const float maxValue)
			{
				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<float> dist(minValue, maxValue);
				return dist(mt);
			}

			static const bool Equals(const sf::Vector2f& vector1, const sf::Vector2f& vector2, const float margin = 0.f)
			{
				return (vector1.x == vector2.x + margin || vector1.x == vector2.x - margin)
					&& (vector1.y == vector2.y + margin || vector1.y == vector2.y - margin);
			}
		}
	}
}