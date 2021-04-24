#pragma once

#include "CollideableTile.h"

class DeadlyTile : public CollideableTile
{
public:
	DeadlyTile(float xCenterPos, float yCenterPos, float width, float height);
};