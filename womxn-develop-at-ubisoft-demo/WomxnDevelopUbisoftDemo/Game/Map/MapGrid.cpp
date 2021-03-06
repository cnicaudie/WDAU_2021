#include <stdafx.h>
#include "MapGrid.h"

MapGrid::MapGrid(const sf::Vector2u& tileSize)
	: m_TileSize(tileSize)
{}

const sf::Vector2i MapGrid::GetTileCoordinates(const sf::Vector2f& position) const
{
	sf::Vector2i result{ static_cast<int>(position.x / m_TileSize.x), static_cast<int>(position.y / m_TileSize.y) };
	
	// Check if the position is at the intersection between two tiles on X
	if (std::fmod(position.x, m_TileSize.x) == 0.)
	{
		result.x -= 1;
	}

	// Check if the position is at the intersection between two tiles on Y
	if (std::fmod(position.y, m_TileSize.y) == 0.)
	{
		result.y -= 1;
	}

	result.x = std::clamp(result.x, 0, static_cast<int>(m_TileGrid.size() - 1));
	result.y = std::clamp(result.y, 0, static_cast<int>(m_TileGrid[0].size() - 1));

	return result;
}

const std::vector<std::shared_ptr<Tile>> MapGrid::GetBoundingTiles(const sf::FloatRect& boundingBox) const
{
	std::vector<std::shared_ptr<Tile>> result;

	sf::Vector2i startPoint = GetTileCoordinates({ boundingBox.left, boundingBox.top });
	sf::Vector2i endPoint = GetTileCoordinates({ boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height });
	
	for (int i = startPoint.x; i <= endPoint.x; i++)
	{
		for (int j = startPoint.y; j <= endPoint.y; j++)
		{
			result.push_back(GetTileAt(i, j));
		}
	}

	return result;
}

void MapGrid::SetCollideableOnTiles(BoxCollideable& collider)
{
	for (std::shared_ptr<Tile> tile : GetBoundingTiles(collider.GetBoundingBox())) 
	{
		tile->AddCollideable(collider);
	}
}

void MapGrid::RemoveCollideableOnTiles(BoxCollideable& collider)
{
	for (std::shared_ptr<Tile> tile : GetBoundingTiles(collider.GetBoundingBox()))
	{
		tile->RemoveCollideable(collider);
	}
}