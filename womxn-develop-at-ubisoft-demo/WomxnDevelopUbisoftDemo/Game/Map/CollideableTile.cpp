#include <stdafx.h>
#include "CollideableTile.h"

CollideableTile::CollideableTile(float xCenterPos, float yCenterPos, float width, float height) 
	: Tile(xCenterPos, yCenterPos, width, height)
{
	SetTrigger(false);
}