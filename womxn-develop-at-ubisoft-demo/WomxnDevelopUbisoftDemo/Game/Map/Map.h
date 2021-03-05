#pragma once

#include "Tile.h"
#include <Game/Objects/SoulChunk.h>
#include <Game/Map/MapGrid.h>

class Map : public sf::Drawable, public sf::Transformable
{
public:
    Map(const sf::Texture& tileSet);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void Update(float deltaTime);
    bool Load(const std::vector<int>& tiles, const sf::Vector2u& levelSize);

    inline const std::vector<SoulChunk>& GetSoulChunks() const { return m_SoulChunks; }

private:
    void DefineTileMapVertice(unsigned int i, unsigned int j, const sf::Vector2u& levelSize, int tu, int tv);
    void DefineMapGrid(int tileNumber, unsigned int i, unsigned int j, const sf::Vector2u& levelSize);
    
    MapGrid m_MapGrid;

    sf::Texture m_TileSet;
    sf::VertexArray m_BackgroundTileMap;

    // ==== Other objects on the map
    
    std::vector<SoulChunk> m_SoulChunks;
};