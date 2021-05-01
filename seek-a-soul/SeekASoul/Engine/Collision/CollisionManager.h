#pragma once

#include <Engine/Collision/GameGrid.h>
#include "CollisionDirection.h"

namespace SeekASoul
{
	namespace Engine
	{
		class CollisionManager 
		{
		public:
			CollisionManager();

			const std::pair<bool, bool> CheckCollisions(BoxCollideable* collideable, const sf::Vector2f& positionOffset, const GameGrid& grid) const;
			const CollisionDirection GetCollisionDirection(BoxCollideable* collideable, BoxCollideable* collider) const;
		};
	}
}