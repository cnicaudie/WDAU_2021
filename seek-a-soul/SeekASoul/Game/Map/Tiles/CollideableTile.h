#pragma once

#include "Tile.h"

namespace SeekASoul
{
	namespace Gameplay
	{
		class CollideableTile : public Tile
		{
		public:
			CollideableTile(float xCenterPos, float yCenterPos, float width, float height);
		};
	}
}