#pragma once

#include <Game/Player.h>
#include <Game/Map/TileMap.h>

class CollisionManager 
{
public:
	CollisionManager();

	const bool CheckCollisionX(Player& player, const sf::Vector2f& nextPosition, const TileMap& map) const;
	const bool CheckCollisionY(Player& player, const sf::Vector2f& nextPosition, const TileMap& map) const;

};