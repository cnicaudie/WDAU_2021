#pragma once

#include <Game/Enemy.h>
#include <Game/Player.h>
#include <Game/Objects/Bullet.h>
#include <Game/Objects/SoulChunk.h>
#include <Game/Map/TileMap.h>

class CollisionManager 
{
public:
	CollisionManager();

	const bool CheckPlayerCollisionX(Player& player, const sf::Vector2f& nextPosition, const TileMap& map) const;
	const bool CheckPlayerCollisionY(Player& player, const sf::Vector2f& nextPosition, const TileMap& map) const;

	const bool CheckBulletCollisionWithEnemies(const Bullet& bullet, std::vector<Enemy>& enemies) const;
	const bool CheckBulletCollisionWithMap(const Bullet& bullet, const TileMap& map) const;

	inline const bool CheckPlayerTriggerWithSoulChunk(const Player& player, const SoulChunk& soulChunk) const 
	{
		return soulChunk.Contains(player.GetCenter());
	}
};