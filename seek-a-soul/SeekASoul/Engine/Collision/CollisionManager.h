#pragma once

#include <Game/Map/GameGrid.h>

class CollisionManager 
{
public:
	CollisionManager();

	const std::pair<bool, bool> CheckCollisions(BoxCollideable* collideable, const sf::Vector2f& positionOffset, const GameGrid& mapGrid) const;
	const CollisionDirection GetCollisionDirection(BoxCollideable* collideable, BoxCollideable* collider) const;
};