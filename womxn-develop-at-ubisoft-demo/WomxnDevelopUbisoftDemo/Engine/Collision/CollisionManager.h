#pragma once

#include <Game/Map/MapGrid.h>

class CollisionManager 
{
public:
	CollisionManager();

	const bool CheckCollision(BoxCollideable* first, const sf::Vector2f& positionOffset, const MapGrid& mapGrid) const;

	/*
	const bool CheckBulletCollisionWithEnemies(const Bullet& bullet, std::vector<Enemy>& enemies) const;
*/
};