#pragma once

#include <Engine/Collision/GameGrid.h>
#include <Game/Entities/Player.h>

namespace SeekASoul
{
    namespace Gameplay
    {
        class Enemy;
        class Door;
        class SoulChunk;
        class MovingPlatform;

        class GameMap : public sf::Drawable, public sf::Transformable
        {
        public:
            static const sf::Vector2u TILE_SIZE;

            GameMap(const std::shared_ptr<Engine::InputManager>& inputManager, const std::shared_ptr<Engine::TextureManager>& textureManager);
            ~GameMap();

            void Update(float deltaTime);
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void RenderDebugMenu(sf::RenderTarget& target);

            bool LoadTileMap(const std::vector<int>& tiles, const sf::Vector2u& levelSize);
            void InitObjectsAndEntities(const std::map<std::string, std::vector<std::string>>& configKeymap, bool restart);

            inline const Engine::GameGrid& GetGameGrid() const { return m_MapGrid; };
            inline const Player& GetPlayer() const { return m_Player; };

        private:
            void UpdateEnemies(float deltaTime);
            void UpdateSoulChunks(float deltaTime);
            void UpdateMovingPlatforms(float deltaTime);
    
            void CreateVertexQuad(unsigned int i, unsigned int j, const sf::Vector2u& levelSize, int tu, int tv);
            void CreateTile(int tileNumber, std::vector<std::shared_ptr<Engine::BoxCollideable>>& tileLine, unsigned int i, unsigned int j, const sf::Vector2u& levelSize);
    
            void InitPlayer(const std::map<std::string, std::vector<std::string>>& configKeymap, bool restart);
            void InitEnemies(const std::map<std::string, std::vector<std::string>>& configKeymap);
            void InitSoulChunks(const std::map<std::string, std::vector<std::string>>& configKeymap);
            void InitMovingPlatforms(const std::map<std::string, std::vector<std::string>>& configKeymap);
            void InitDoor(const std::map<std::string, std::vector<std::string>>& configKeymap);
    
            //====================//
    
            std::shared_ptr<Engine::TextureManager> m_TextureManager;

            Engine::GameGrid m_MapGrid;
            sf::Texture m_TileSet;
            sf::VertexArray m_BackgroundTileMap;

            // ==== Other objects / entities on the map
    
            Player m_Player;
            std::shared_ptr<Door> m_Door;
            std::vector<std::shared_ptr<Enemy>> m_Enemies;
            std::vector<std::shared_ptr<SoulChunk>> m_SoulChunks;
            std::vector<std::shared_ptr<MovingPlatform>> m_MovingPlatforms;
        };
    }
}