#include <stdafx.h>
#include "DeadlyTile.h"

namespace SeekASoul
{
	namespace Gameplay
	{
		DeadlyTile::DeadlyTile(float xCenterPos, float yCenterPos, float width, float height)
			: CollideableTile(xCenterPos, yCenterPos, width, height)
		{}
	}
}