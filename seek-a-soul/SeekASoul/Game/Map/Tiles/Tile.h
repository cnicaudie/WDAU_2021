#pragma once

namespace SeekASoul
{
	namespace Gameplay
	{
		class Tile : public Engine::BoxCollideable
		{
		public:
			Tile(float xCenterPos, float yCenterPos, float width, float height);
		};
	}
}