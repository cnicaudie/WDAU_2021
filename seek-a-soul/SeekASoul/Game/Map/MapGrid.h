#pragma once

class Tile;

class MapGrid 
{
friend class Map;

public:
	MapGrid(const sf::Vector2u& tileSize);
	~MapGrid() = default;

	const std::vector<std::shared_ptr<Tile>> GetNearbyTiles(const sf::FloatRect& boundingBox) const;
	const std::vector<BoxCollideable*> GetNearbyObjects(const sf::FloatRect& boundingBox) const;

	inline void AddObjectOnMap(BoxCollideable* object) 
	{
		m_ObjectsOnMapX.insert(object);
		m_ObjectsOnMapY.insert(object);
	}

private:
	const sf::Vector2i GetTileCoordinates(const sf::Vector2f& position) const;
	
	inline const std::shared_ptr<Tile> GetTileAt(int x, int y) const { return m_TileGrid[x][y]; };
	
	//====================//

	sf::Vector2u m_TileSize;
	std::vector<std::vector<std::shared_ptr<Tile>>> m_TileGrid;
	
	struct BoxComparatorX 
	{
		bool operator()(const BoxCollideable* a, const BoxCollideable* b) const 
		{
			return a->GetCenter().x < b->GetCenter().x;
		}
	};

	struct BoxComparatorY
	{
		bool operator()(const BoxCollideable* a, const BoxCollideable* b) const
		{
			return a->GetCenter().y < b->GetCenter().y;
		}
	};

	// Sorted objects by their position on X and Y axis
	std::set<BoxCollideable*, BoxComparatorX> m_ObjectsOnMapX;
	std::set<BoxCollideable*, BoxComparatorY> m_ObjectsOnMapY;

};