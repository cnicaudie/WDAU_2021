#include <stdafx.h>
#include "Tile.h"

namespace SeekASoul
{
	namespace Gameplay
	{
		Tile::Tile(float xCenterPos, float yCenterPos, float width, float height)
		{
			const auto center = sf::Vector2f(xCenterPos, yCenterPos);
			const auto size = sf::Vector2f(width, height);
			SetBoundingBox(center, size);
			SetTrigger(true);
		}
	}
}