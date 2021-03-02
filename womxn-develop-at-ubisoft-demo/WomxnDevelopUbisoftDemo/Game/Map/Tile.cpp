#include <stdafx.h>
#include "Tile.h"

Tile::Tile(float xCenterPos, float yCenterPos, TileType tileType, float width, float height) 
	: m_TileType(tileType)
{
	const auto center = sf::Vector2f(xCenterPos, yCenterPos);
	const auto size = sf::Vector2f(width, height);
	SetBoundingBox(center, size);
}