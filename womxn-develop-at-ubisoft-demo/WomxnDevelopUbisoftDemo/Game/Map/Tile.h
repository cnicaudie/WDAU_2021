#pragma once

#include "TileType.h"
#include <typeinfo>

class Tile : public BoxCollideable
{
public:
	Tile(float xCenterPos, float yCenterPos, float width, float height);

	inline void AddCollideable(const BoxCollideable& collideable)
	{
		m_CollideablesOnTile.push_back(std::make_shared<BoxCollideable>(collideable));
	};

	inline void RemoveCollideable(const BoxCollideable& collideable)
	{
		// TODO : See if there's a better way to compare 2 BoxCollideables
		auto it = std::find_if(m_CollideablesOnTile.begin(), m_CollideablesOnTile.end(), [collideable](std::shared_ptr<BoxCollideable> ptr) {
			return ptr->GetBoundingBox() == collideable.GetBoundingBox(); });

		if (it != m_CollideablesOnTile.end()) 
		{
			m_CollideablesOnTile.erase(it);
		}
	};

	inline const std::vector<std::shared_ptr<BoxCollideable>>& GetCollideablesOnTile() const { return m_CollideablesOnTile; }

private:
	// Contains all collideables that are on this Tile (entities, objects...)
	std::vector<std::shared_ptr<BoxCollideable>> m_CollideablesOnTile;
};