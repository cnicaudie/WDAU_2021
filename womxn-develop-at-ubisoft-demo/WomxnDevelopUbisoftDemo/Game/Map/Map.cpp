#include <stdafx.h>
#include "Map.h"
#include "CollideableTile.h"

const sf::Vector2u Map::TILE_SIZE{ 32, 32 };

Map::Map(const std::shared_ptr<TextureManager>& textureManager)
    : m_TileSet(textureManager->GetTextureFromName("TILESET"))
    , m_MapGrid(TILE_SIZE)
    , m_Door{ 1200, 120, 50, 100 }
{
    m_SoulChunks.emplace_back(m_TileSet, sf::Vector2f(336.f, 208.f));
    m_Enemies.emplace_back(textureManager);
    std::cout << "Map created !" << std::endl;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Draw the background map
    states.transform *= getTransform(); // Apply the transform
    states.texture = &m_TileSet; // Apply the tileset texture
    target.draw(m_BackgroundTileMap, states); // Draw the vertex array

    // Draw other objects
    for (const SoulChunk& s : m_SoulChunks)
    {
        if (!s.WasCollected()) 
        {
            target.draw(s);
        }
    }

    target.draw(m_Door);

    // Draw entities
    for (const Enemy& enemy : m_Enemies)
    {
        target.draw(enemy);
    }
}

void Map::Update(float deltaTime) 
{
    // Update Soul Chunks
    int soulIndex = 0;
    for (SoulChunk& s : m_SoulChunks)
    {
        s.Update(deltaTime);
        if (s.WasCollected())
        {
            m_MapGrid.RemoveCollideableOnTiles(s);
            m_SoulChunks.erase(m_SoulChunks.begin() + soulIndex);
        }
        soulIndex++;
    }

    // Update Door
    if (!m_Door.IsPlayingEndGame()) 
    {
        m_Door.Update(deltaTime);
    } 
    else 
    {
        m_MapGrid.RemoveCollideableOnTiles(m_Door);
    }

    // Update Enemies
    for (Enemy& enemy : m_Enemies)
    {
        if (!enemy.IsDead()) 
        {
            enemy.Update(deltaTime);
        } 
        else 
        {
            m_MapGrid.RemoveCollideableOnTiles(enemy);
        }
    }
}

bool Map::Load(const std::vector<int>& tiles, const sf::Vector2u& levelSize)
{
    // Resize the vertex array to fit the level size
    m_BackgroundTileMap.setPrimitiveType(sf::Quads);
    m_BackgroundTileMap.resize(static_cast<size_t>(levelSize.x) * static_cast<size_t>(levelSize.y) * 4);
    
    // Clear the map grid if necessary
    if (!m_MapGrid.m_TileGrid.empty())
    {
        m_MapGrid.m_TileGrid.clear();
    }

    // Populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < levelSize.x; ++i) 
    {
        std::vector<std::shared_ptr<Tile>> tileLine;
        for (unsigned int j = 0; j < levelSize.y; ++j)
        {
            // Get the current tile number
            int tileNumber = tiles[static_cast<size_t>(i) + static_cast<size_t>(j) * static_cast<size_t>(levelSize.x)];

            // Find its position in the tileset texture
            int tu = tileNumber % (m_TileSet.getSize().x / TILE_SIZE.x);
            int tv = tileNumber / (m_TileSet.getSize().x / TILE_SIZE.x);

            CreateVertexQuad(i, j, levelSize, tu, tv);
            CreateTile(tileNumber, tileLine, i, j, levelSize);
        }
        m_MapGrid.m_TileGrid.push_back(tileLine);
    }

    LoadObjectsAndEntities();

    return true;
}

void Map::LoadObjectsAndEntities()
{
    for (SoulChunk& soulChunk : m_SoulChunks)
    {
        m_MapGrid.SetCollideableOnTiles(soulChunk);
    }

    m_MapGrid.SetCollideableOnTiles(m_Door);

    for (Enemy& enemy : m_Enemies)
    {
        m_MapGrid.SetCollideableOnTiles(enemy);
    }
}

void Map::CreateVertexQuad(unsigned int i, unsigned int j, const sf::Vector2u& levelSize, int tu, int tv)
{
    // Get a pointer to the current tile's quad (of the vertex array)
    sf::Vertex* quad = &m_BackgroundTileMap[(static_cast<size_t>(i) + static_cast<size_t>(j) * static_cast<size_t>(levelSize.x)) * 4];

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
}

void Map::CreateTile(int tileNumber, std::vector<std::shared_ptr<Tile>>& tileLine, unsigned int i, unsigned int j, const sf::Vector2u& levelSize)
{
    TileType tileType = static_cast<TileType>(tileNumber);
    unsigned int xCenter = (TILE_SIZE.x / 2) + i * TILE_SIZE.x;
    unsigned int yCenter = (TILE_SIZE.y / 2) + j * TILE_SIZE.y;

    switch (tileType)
    {
    case TileType::CONCRETE:
    {
        tileLine.push_back(std::make_shared<CollideableTile>
            (static_cast<float>(xCenter)
                , static_cast<float>(yCenter)
                , static_cast<float>(TILE_SIZE.x)
                , static_cast<float>(TILE_SIZE.y)
                ));
        break;
    }

    default:
    {
        tileLine.push_back(std::make_shared<Tile>
            (static_cast<float>(xCenter)
                , static_cast<float>(yCenter)
                , static_cast<float>(TILE_SIZE.x)
                , static_cast<float>(TILE_SIZE.y)
                ));
        break;
    }
    }
}
