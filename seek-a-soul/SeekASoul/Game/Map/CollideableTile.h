#pragma once
#include "Tile.h"

class CollideableTile : public Tile
{
public:
	CollideableTile(float xCenterPos, float yCenterPos, float width, float height);
};