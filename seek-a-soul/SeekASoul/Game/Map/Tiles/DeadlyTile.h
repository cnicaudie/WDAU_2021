#pragma once

#include "CollideableTile.h"

namespace SeekASoul
{
	namespace Gameplay
	{
		class DeadlyTile : public CollideableTile
		{
		public:
			DeadlyTile(float xCenterPos, float yCenterPos, float width, float height);
		};
	}
}