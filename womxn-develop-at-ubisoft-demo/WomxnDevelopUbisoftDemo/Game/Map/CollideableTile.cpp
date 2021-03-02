#include <stdafx.h>
#include "CollideableTile.h"

CollideableTile::CollideableTile(TileType tileType, float xCenterPos, float yCenterPos, float width, float height) 
	: Tile(tileType)
{
	const auto center = sf::Vector2f(xCenterPos, yCenterPos);
	const auto size = sf::Vector2f(width, height);
	SetBoundingBox(center, size);
}