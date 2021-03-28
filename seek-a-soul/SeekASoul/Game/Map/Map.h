#pragma once

#include <Game/Map/Tiles/Tile.h>
#include <Game/Map/MapGrid.h>
#include <Game/Objects/SoulChunk.h>
#include <Game/Objects/Door.h>
#include <Game/Entities/Enemy.h>

class Map : public sf::Drawable, public sf::Transformable
{
public:
    static const sf::Vector2u TILE_SIZE;

    Map(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void Update(float deltaTime);
    bool Load(const std::vector<int>& tiles, const sf::Vector2u& levelSize);

    inline const std::vector<SoulChunk>& GetSoulChunks() const { return m_SoulChunks; };
    inline const MapGrid& GetMapGrid() const { return m_MapGrid; };
    inline const Player& GetPlayer() const { return m_Player; };

private:
    void LoadObjectsAndEntities();
    void CreateVertexQuad(unsigned int i, unsigned int j, const sf::Vector2u& levelSize, int tu, int tv);
    void CreateTile(int tileNumber, std::vector<std::shared_ptr<Tile>>& tileLine, unsigned int i, unsigned int j, const sf::Vector2u& levelSize);
    
    //====================//
    
    MapGrid m_MapGrid;
    sf::Texture m_TileSet;
    sf::VertexArray m_BackgroundTileMap;

    // ==== Other objects / entities on the map
    
    std::vector<SoulChunk> m_SoulChunks;
    Door m_Door;

    std::vector<Enemy> m_Enemies;
    Player m_Player;
};