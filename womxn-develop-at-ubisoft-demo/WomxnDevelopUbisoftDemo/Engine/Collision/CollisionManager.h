#pragma once

#include <Game/Player.h>
#include <Game/Map/TileMap.h>

class CollisionManager 
{
public:
	CollisionManager();

	const bool CheckPlayerCollisionX(Player& player, const sf::Vector2f& nextPosition, const TileMap& map) const;
	const bool CheckPlayerCollisionY(Player& player, const sf::Vector2f& nextPosition, const TileMap& map) const;

};