#pragma once

#include <Game/Enemy.h>
#include <Game/Player.h>
#include <Game/Objects/Bullet.h>
#include <Game/Objects/SoulChunk.h>
#include <Game/Map/Map.h>

class CollisionManager 
{
public:
	CollisionManager();

	const bool CheckCollision(const BoxCollideable& first, const sf::Vector2f& nextPosition, const std::vector<BoxCollideable>& others) const;

	/*const bool CheckPlayerCollisionX(Player& player, const sf::Vector2f& nextPosition, const Map& map) const;
	const bool CheckPlayerCollisionY(Player& player, const sf::Vector2f& nextPosition, const Map& map) const;

	const bool CheckBulletCollisionWithEnemies(const Bullet& bullet, std::vector<Enemy>& enemies) const;
	const bool CheckBulletCollisionWithMap(const Bullet& bullet, const Map& map) const;

	inline const bool CheckPlayerTriggerWithSoulChunk(const Player& player, const SoulChunk& soulChunk) const 
	{
		return soulChunk.Contains(player.GetCenter());
	}*/
};