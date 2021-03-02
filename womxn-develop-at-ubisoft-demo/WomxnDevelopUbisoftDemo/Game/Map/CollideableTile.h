#pragma once

#include "Tile.h"

class CollideableTile : public Tile, public BoxCollideable 
{
public:
	CollideableTile(TileType tileType, float xCenterPos, float yCenterPos, float width, float height);
};