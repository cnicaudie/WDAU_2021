#pragma once

#include <Game/Map/MapGrid.h>
#include <Game/Objects/Door.h>
#include <Game/Entities/Enemy.h>
#include <Game/Entities/Player.h>

class Tile;
class SoulChunk;

class Map : public sf::Drawable, public sf::Transformable
{
public:
    static const sf::Vector2u TILE_SIZE;

    Map(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager);
    ~Map();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void Update(float deltaTime);
    
    bool LoadTileMap(const std::vector<int>& tiles, const sf::Vector2u& levelSize);
    void InitObjectsAndEntities(const std::map<std::string, std::vector<std::string>>& configKeymap);

    inline const MapGrid& GetMapGrid() const { return m_MapGrid; };
    inline const Player& GetPlayer() const { return m_Player; };

private:
    void UpdateEnemies(float deltaTime);
    void UpdateSoulChunks(float deltaTime);
    
    void CreateVertexQuad(unsigned int i, unsigned int j, const sf::Vector2u& levelSize, int tu, int tv);
    void CreateTile(int tileNumber, std::vector<std::shared_ptr<Tile>>& tileLine, unsigned int i, unsigned int j, const sf::Vector2u& levelSize);
    
    void InitDoor(const std::map<std::string, std::vector<std::string>>& configKeymap);
    void InitPlayer(const std::map<std::string, std::vector<std::string>>& configKeymap);
    void InitEnemies(const std::map<std::string, std::vector<std::string>>& configKeymap);
    void InitSoulChunks(const std::map<std::string, std::vector<std::string>>& configKeymap);
    
    //====================//
    
    std::shared_ptr<TextureManager> m_TextureManager;

    MapGrid m_MapGrid;
    sf::Texture m_TileSet;
    sf::VertexArray m_BackgroundTileMap;

    // ==== Other objects / entities on the map
    
    std::vector<std::unique_ptr<SoulChunk>> m_SoulChunks;
    Door m_Door;

    std::vector<Enemy> m_Enemies;
    Player m_Player;
};