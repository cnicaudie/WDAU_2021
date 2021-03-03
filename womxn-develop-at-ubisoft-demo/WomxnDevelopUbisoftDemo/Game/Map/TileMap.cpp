#include <stdafx.h>
#include "TileMap.h"
#include "CollideableTile.h"

static const sf::Vector2u TILE_SIZE{ 32, 32 };

TileMap::TileMap() : m_soulChunk(m_Tileset, sf::Vector2f(336.f, 208.f)) {}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Apply the transform
    states.transform *= getTransform();

    // Apply the tileset texture
    states.texture = &m_Tileset;

    // Draw the vertex array
    target.draw(m_Vertices, states);

    target.draw(m_soulChunk);
}

bool TileMap::Load(const sf::Texture& tileset, const std::vector<int>& tiles, const sf::Vector2u& levelSize)
{
    m_Tileset = tileset;

    // Resize the vertex array to fit the level size
    m_Vertices.setPrimitiveType(sf::Quads);
    m_Vertices.resize(static_cast<size_t>(levelSize.x) * static_cast<size_t>(levelSize.y) * 4);
    
    // Populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < levelSize.x; ++i)
        for (unsigned int j = 0; j < levelSize.y; ++j)
        {
            // Get the current tile number
            int tileNumber = tiles[static_cast<size_t>(i) + static_cast<size_t>(j) * static_cast<size_t>(levelSize.x)];

            // Find its position in the tileset texture
            int tu = tileNumber % (m_Tileset.getSize().x / TILE_SIZE.x);
            int tv = tileNumber / (m_Tileset.getSize().x / TILE_SIZE.x);

            // Get a pointer to the current tile's quad
            sf::Vertex* quad = &m_Vertices[(static_cast<size_t>(i) + static_cast<size_t>(j) * static_cast<size_t>(levelSize.x)) * 4];
            
            // Define its 4 corners
            quad[0].position = sf::Vector2f(static_cast<float>(i * TILE_SIZE.x), static_cast<float>(j * TILE_SIZE.y));
            quad[1].position = sf::Vector2f(static_cast<float>((i + 1) * TILE_SIZE.x), static_cast<float>(j * TILE_SIZE.y));
            quad[2].position = sf::Vector2f(static_cast<float>((i + 1) * TILE_SIZE.x), static_cast<float>((j + 1) * TILE_SIZE.y));
            quad[3].position = sf::Vector2f(static_cast<float>(i * TILE_SIZE.x), static_cast<float>((j + 1) * TILE_SIZE.y));

            // Define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(static_cast<float>(tu * TILE_SIZE.x), static_cast<float>(tv * TILE_SIZE.y));
            quad[1].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * TILE_SIZE.x), static_cast<float>(tv * TILE_SIZE.y));
            quad[2].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * TILE_SIZE.x), static_cast<float>((tv + 1) * TILE_SIZE.y));
            quad[3].texCoords = sf::Vector2f(static_cast<float>(tu * TILE_SIZE.x), static_cast<float>((tv + 1) * TILE_SIZE.y));

            // Add the new tile to the tilemap vector
            unsigned int xCenter = (TILE_SIZE.x / 2) + i * TILE_SIZE.x;
            unsigned int yCenter = (TILE_SIZE.y / 2) + j * TILE_SIZE.y;

            switch (tileNumber)
            {
                // TODO : Add other cases or remove TileType
                case 3:
                {
                    m_TileMap.emplace_back(std::make_shared<CollideableTile>(TileType::CONCRETE, static_cast<float>(xCenter), static_cast<float>(yCenter), TILE_SIZE.x, TILE_SIZE.y));
                    break;
                }
                
                case 4:
                {
                    m_TileMap.emplace_back(std::make_shared<Tile>(TileType::EMPTY));
                    break;
                }

                default:
                {
                    m_TileMap.emplace_back(std::make_shared<Tile>(static_cast<TileType>(tileNumber)));
                    break;
                }

            }
        }

    return true;
}