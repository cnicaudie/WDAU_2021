#include <stdafx.h>
#include "TileMap.h"

TileMap::TileMap() 
{
    // TODO : Init the map
}

bool TileMap::LoadTileset(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
{
    // TODO : Move in TextureManager
    // load the tileset texture 
    if (!m_Tileset.loadFromFile(tileset))
        return false;

    // resize the vertex array to fit the level size
    m_Vertices.setPrimitiveType(sf::Quads);
    m_Vertices.resize(static_cast<size_t>(width) * static_cast<size_t>(height) * 4);
    
    // populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = tiles[i + j * width];

            // find its position in the tileset texture
            int tu = tileNumber % (m_Tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_Tileset.getSize().x / tileSize.x);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_Vertices[(static_cast<size_t>(i) + static_cast<size_t>(j) * static_cast<size_t>(width)) * 4];
            
            // define its 4 corners
            quad[0].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>(j * tileSize.y));
            quad[1].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>(j * tileSize.y));
            quad[2].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>((j + 1) * tileSize.y));
            quad[3].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>((j + 1) * tileSize.y));

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(static_cast<float>(tu * tileSize.x), static_cast<float>(tv * tileSize.y));
            quad[1].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * tileSize.x), static_cast<float>(tv * tileSize.y));
            quad[2].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * tileSize.x), static_cast<float>((tv + 1) * tileSize.y));
            quad[3].texCoords = sf::Vector2f(static_cast<float>(tu * tileSize.x), static_cast<float>((tv + 1) * tileSize.y));
        }

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_Tileset;

    // draw the vertex array
    target.draw(m_Vertices, states);
}