#pragma once

#include "TileType.h"
#include <typeinfo>

class Tile : public BoxCollideable
{
public:
	Tile(float xCenterPos, float yCenterPos, float width, float height);

	inline void AddCollideable(BoxCollideable& collideable)
	{
		m_CollideablesOnTile.push_back(&collideable);
	};

	inline void RemoveCollideable(const BoxCollideable& collideable)
	{
		// TODO : See if there's a better way to compare 2 BoxCollideables
		auto it = std::find_if(m_CollideablesOnTile.begin(), m_CollideablesOnTile.end(), [collideable](BoxCollideable* ptr) {
			return ptr->GetBoundingBox() == collideable.GetBoundingBox(); });

		if (it != m_CollideablesOnTile.end()) 
		{
			m_CollideablesOnTile.erase(it);
		}
	};

	inline const std::vector<BoxCollideable*>& GetCollideablesOnTile() const { return m_CollideablesOnTile; }

private:
	// Contains all collideables that are on this Tile (entities, objects...)
	std::vector<BoxCollideable*> m_CollideablesOnTile;
};