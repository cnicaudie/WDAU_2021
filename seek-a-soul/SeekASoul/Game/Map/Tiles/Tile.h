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
		const auto& it = std::find_if(m_CollideablesOnTile.begin(), m_CollideablesOnTile.end(), [&](BoxCollideable* ptr) {
			return (*ptr) == collideable; });

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