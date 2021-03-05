#include <stdafx.h>
#include "MapGrid.h"

MapGrid::MapGrid(const sf::Vector2u& tileSize)
	: m_TileSize(tileSize)
{
	//m_TileGrid.resize(levelSize.x, std::vector<std::shared_ptr<Tile>>(levelSize.y));
}

const std::vector<Tile> MapGrid::GetBoundingTiles(const sf::FloatRect& boundingBox) const
{
	std::vector<Tile> result;

	sf::Vector2i startPoint = GetTileCoordinates({ boundingBox.left, boundingBox.top });
	sf::Vector2i endPoint = GetTileCoordinates({ boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height });

	for (int i = startPoint.x; i < endPoint.x; i++)
	{
		for (int j = startPoint.y; i < endPoint.y; j++)
		{
			result.push_back(GetTileAt(i, j));
		}
	}

	return result;
}