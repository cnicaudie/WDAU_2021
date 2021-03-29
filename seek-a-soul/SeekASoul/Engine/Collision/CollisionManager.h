#pragma once

#include <Game/Map/MapGrid.h>

class CollisionManager 
{
public:
	CollisionManager();

	const bool CheckCollision(BoxCollideable* collideable, const sf::Vector2f& positionOffset, const MapGrid& mapGrid) const;
	const CollisionDirection GetCollisionDirection(BoxCollideable* collideable, BoxCollideable* collider) const;
};