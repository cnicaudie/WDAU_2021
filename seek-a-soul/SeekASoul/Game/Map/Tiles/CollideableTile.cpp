#include <stdafx.h>
#include "CollideableTile.h"

namespace SeekASoul
{
	namespace Gameplay
	{
		CollideableTile::CollideableTile(float xCenterPos, float yCenterPos, float width, float height) 
			: Tile(xCenterPos, yCenterPos, width, height)
		{
			SetTrigger(false);
		}
	}
}