#include <stdafx.h>
#include "GameGrid.h"

namespace SeekASoul
{
	namespace Engine
	{
		GameGrid::GameGrid(const sf::Vector2u& tileSize)
			: m_TileSize(tileSize)
		{}

		const sf::Vector2i GameGrid::GetTileCoordinates(const sf::Vector2f& position) const
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

		const std::vector<std::shared_ptr<BoxCollideable>> GameGrid::GetNearbyTiles(const sf::FloatRect& boundingBox) const
		{
			std::vector<std::shared_ptr<BoxCollideable>> result;

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

		const std::vector<std::shared_ptr<BoxCollideable>> GameGrid::GetNearbyObjects(const sf::FloatRect& boundingBox) const
		{
			std::vector<std::shared_ptr<BoxCollideable>> result;

			// Retrieve X and Y limits of the bounding box
			const float xMin = boundingBox.left;
			const float xMax = boundingBox.left + boundingBox.width;
			const float yMin = boundingBox.top;
			const float yMax = boundingBox.top + boundingBox.height;

			// Find closest box to xMin and yMin in both set
			auto const& posX = std::find_if(m_ObjectsOnGridX.begin(), m_ObjectsOnGridX.end(), [&](std::shared_ptr<BoxCollideable> boxPtr)
				{
					// If left side of the object is outside, we still take it into account
					return boxPtr->GetBoundingBox().left + boxPtr->GetBoundingBox().width > xMin;
				});

			auto const& posY = std::find_if(m_ObjectsOnGridY.begin(), m_ObjectsOnGridY.end(), [&](std::shared_ptr<BoxCollideable> boxPtr)
				{
					// If top side of the object is outside, we still take it into account
					return boxPtr->GetBoundingBox().top + boxPtr->GetBoundingBox().height > yMin;
				});

			// Add objects between the bounds of the bounding box to the nearby objects vector
			if (posX != m_ObjectsOnGridX.end())
			{
				for (auto it = posX; it != m_ObjectsOnGridX.end(); it++)
				{
					// If left side of the object is still inside, we still take it into account
					if ((*it)->GetBoundingBox().left < xMax)
					{
						result.push_back(*it);
					}
				}
			}

			if (posY != m_ObjectsOnGridY.end())
			{
				for (auto it = posY; it != m_ObjectsOnGridY.end(); it++)
				{
					// If top side of the object is still inside, we still take it into account
					if ((*it)->GetBoundingBox().top < yMax)
					{
						// If not already in the result vector, we push the object in
						if (std::find(result.begin(), result.end(), *it) == result.end()) 
						{ 
							result.push_back(*it);
						}
					}
				}
			}

			return result;
		}
	}
}