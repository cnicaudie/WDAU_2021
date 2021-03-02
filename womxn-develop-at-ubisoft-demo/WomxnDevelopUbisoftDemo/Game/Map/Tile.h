#pragma once

#include "TileType.h"

class Tile : public BoxCollideable
{
public:
	Tile(float xCenterPos, float yCenterPos, TileType tileType, float width, float height);

	const TileType GetType() const { return m_TileType; }

private:
	TileType m_TileType;
};