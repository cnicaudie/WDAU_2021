#pragma once

#include "Tile.h"

class MapGrid 
{
friend class Map;

public:
	MapGrid(const sf::Vector2u& tileSize);

	inline const std::shared_ptr<Tile>& GetTileAt(int x, int y) const { return m_TileGrid[x][y]; };

	inline const sf::Vector2i GetTileCoordinates(const sf::Vector2f& position) const 
	{
		sf::Vector2i result{ static_cast<int>(position.x / m_TileSize.x), static_cast<int>(position.y / m_TileSize.y) };

		// Check if the position is at the intersection between two tiles on X
		if (std::fmod(position.x, m_TileSize.x) == 0.) 
		{
			result.x -= 1;
		}

		// Check if the position is at the intersection between two tiles on X
		if (std::fmod(position.y, m_TileSize.y) == 0.)
		{
			result.y -= 1;
		}

		return result;
	};

	const std::vector<std::shared_ptr<Tile>> GetBoundingTiles(const sf::FloatRect& boundingBox) const;

private:
	std::vector<std::vector<std::shared_ptr<Tile>>> m_TileGrid;
	sf::Vector2u m_TileSize;
};