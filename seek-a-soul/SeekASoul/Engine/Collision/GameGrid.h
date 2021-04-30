#pragma once

namespace SeekASoul
{
	namespace Engine
	{
		class GameGrid 
		{
		public:
			GameGrid(const sf::Vector2u& tileSize);
			~GameGrid() = default;

			const std::vector<std::shared_ptr<BoxCollideable>> GetNearbyTiles(const sf::FloatRect& boundingBox) const;
			const std::vector<std::shared_ptr<BoxCollideable>> GetNearbyObjects(const sf::FloatRect& boundingBox) const;

			inline void AddTileLineOnGrid(std::vector<std::shared_ptr<BoxCollideable>> tileLine)
			{
				m_TileGrid.push_back(tileLine);
			};

			inline void AddObjectOnGrid(std::shared_ptr<BoxCollideable> object)
			{
				m_ObjectsOnGridX.insert(object);
				m_ObjectsOnGridY.insert(object);
			}

			inline void RemoveObjectOnGrid(std::shared_ptr<BoxCollideable> object) 
			{
				auto const& posX = std::find_if(m_ObjectsOnGridX.begin(), m_ObjectsOnGridX.end()
					, [&](std::shared_ptr<BoxCollideable> other)
					{
						return *object == *other;
					});

				if (posX != m_ObjectsOnGridX.end())
				{
					m_ObjectsOnGridX.erase(posX);
				}

				auto const& posY = std::find_if(m_ObjectsOnGridY.begin(), m_ObjectsOnGridY.end()
					, [&](std::shared_ptr<BoxCollideable> other)
					{
						return *object == *other;
					});

				if (posY != m_ObjectsOnGridY.end())
				{
					m_ObjectsOnGridY.erase(posY);
				}
			}

			inline void Clear()
			{
				m_TileGrid.clear();
				m_ObjectsOnGridX.clear();
				m_ObjectsOnGridY.clear();
			};

		private:
			const sf::Vector2i GetTileCoordinates(const sf::Vector2f& position) const;
	
			inline std::shared_ptr<BoxCollideable> GetTileAt(int x, int y) const { return m_TileGrid[x][y]; };
	
			//====================//

			struct BoxComparatorX 
			{
				bool operator()(const std::shared_ptr<BoxCollideable> a, const std::shared_ptr<BoxCollideable> b) const
				{
					return a->GetCenter().x < b->GetCenter().x;
				}
			};

			struct BoxComparatorY
			{
				bool operator()(const std::shared_ptr<BoxCollideable> a, const std::shared_ptr<BoxCollideable> b) const
				{
					return a->GetCenter().y < b->GetCenter().y;
				}
			};

			//====================//
			
			sf::Vector2u m_TileSize;
			std::vector<std::vector<std::shared_ptr<BoxCollideable>>> m_TileGrid;

			// Sorted objects by their position on X and Y axis
			std::set<std::shared_ptr<BoxCollideable>, BoxComparatorX> m_ObjectsOnGridX;
			std::set<std::shared_ptr<BoxCollideable>, BoxComparatorY> m_ObjectsOnGridY;
		};
	}
}