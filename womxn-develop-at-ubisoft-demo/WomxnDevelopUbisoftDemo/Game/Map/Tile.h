#pragma once

#include "TileType.h"

class Tile
{
public:
	Tile(TileType tileType);

	virtual const TileType GetType() const { return m_TileType; }

private:
	TileType m_TileType;
};