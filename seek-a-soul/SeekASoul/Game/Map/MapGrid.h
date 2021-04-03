#pragma once

class Tile;

class MapGrid 
{
friend class Map;

public:
	MapGrid(const sf::Vector2u& tileSize);
	~MapGrid() = default;

	inline const std::shared_ptr<Tile>& GetTileAt(int x, int y) const { return m_TileGrid[x][y]; };

	const sf::Vector2i GetTileCoordinates(const sf::Vector2f& position) const;

	const std::vector<std::shared_ptr<Tile>> GetBoundingTiles(const sf::FloatRect& boundingBox) const;

	void SetCollideableOnTiles(BoxCollideable& collider);
	void RemoveCollideableOnTiles(BoxCollideable& collider);

private:
	std::vector<std::vector<std::shared_ptr<Tile>>> m_TileGrid;
	sf::Vector2u m_TileSize;
};