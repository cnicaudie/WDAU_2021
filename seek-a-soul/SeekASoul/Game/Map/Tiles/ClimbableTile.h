#pragma once

#include "Tile.h"

namespace SeekASoul
{
	namespace Gameplay
	{
		class ClimbableTile : public Tile
		{
		public:
			ClimbableTile(float xCenterPos, float yCenterPos, float width, float height);
		};
	}
}