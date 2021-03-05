#pragma once

#include <Game/Map/MapGrid.h>

class CollisionManager 
{
public:
	CollisionManager();

	const bool CheckCollision(BoxCollideable* first, const sf::Vector2f& nextPosition, const MapGrid& mapGrid) const;

	/*const bool CheckPlayerCollisionX(Player& player, const sf::Vector2f& nextPosition, const Map& map) const;
	const bool CheckPlayerCollisionY(Player& player, const sf::Vector2f& nextPosition, const Map& map) const;

	const bool CheckBulletCollisionWithEnemies(const Bullet& bullet, std::vector<Enemy>& enemies) const;
	const bool CheckBulletCollisionWithMap(const Bullet& bullet, const Map& map) const;

	inline const bool CheckPlayerTriggerWithSoulChunk(const Player& player, const SoulChunk& soulChunk) const 
	{
		return soulChunk.Contains(player.GetCenter());
	}*/
};