#include <stdafx.h>
#include "GameMap.h"
#include <Game/Entities/Enemy.h>
#include <Game/Objects/Door.h>
#include <Game/Objects/Collectibles/SoulChunk.h>
#include <Game/Objects/MovingPlatform.h>
#include <Game/Map/Tiles/Tile.h>
#include <Game/Map/Tiles/TileType.h>
#include <Game/Map/Tiles/CollideableTile.h>
#include <Game/Map/Tiles/ClimbableTile.h>
#include <Game/Map/Tiles/DeadlyTile.h>

namespace SeekASoul
{
    namespace Gameplay
    {
        const sf::Vector2u GameMap::TILE_SIZE{ 32, 32 };

        GameMap::GameMap(const std::shared_ptr<Engine::InputManager>& inputManager, const std::shared_ptr<Engine::TextureManager>& textureManager)
            : m_TextureManager(textureManager)
            , m_TileSet(textureManager->GetTextureFromName("TILESET"))
            , m_MapGrid(TILE_SIZE)
            , m_Player{ inputManager, textureManager }
            , m_Door{ std::make_shared<Door>(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)) }
        {
            LOG_INFO("GameMap created !");
        }

        GameMap::~GameMap() {}

        void GameMap::Update(float deltaTime) 
        {
            // Update Door
            m_Door->Update(deltaTime);

            // Update Player
            m_Player.Update(deltaTime);
    
            UpdateEnemies(deltaTime);
            UpdateSoulChunks(deltaTime);
            UpdateMovingPlatforms(deltaTime);
        }

        void GameMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            // === Draw the background map
            states.transform *= getTransform(); // Apply the transform
            states.texture = &m_TileSet; // Apply the tileset texture
            target.draw(m_BackgroundTileMap, states); // Draw the vertex array

            // === Draw other objects
            for (const std::shared_ptr<SoulChunk>& s : m_SoulChunks)
            {
                target.draw(*s);
            }

            for (const std::shared_ptr<MovingPlatform>& platform : m_MovingPlatforms)
            {
                target.draw(*platform);
            }

            target.draw(*m_Door);

            // === Draw entities
            for (const std::shared_ptr<Enemy>& enemy : m_Enemies)
            {
                target.draw(*enemy);
            }

            target.draw(m_Player);
        }

        void GameMap::RenderDebugMenu(sf::RenderTarget& target)
        {
            m_Door->RenderDebugMenu(target);
            m_Player.RenderDebugMenu(target);
        }

        void GameMap::UpdateEnemies(float deltaTime)
        {
            for (auto it = m_Enemies.begin(); it < m_Enemies.end(); it++)
            {   
                (*it)->Update(deltaTime);
        
                if ((*it)->IsDead())
                {
                    m_MapGrid.RemoveObjectOnGrid(*it);
                    it = m_Enemies.erase(it);
                    if (it == m_Enemies.end()) { break; }
                }
            }
        }

        void GameMap::UpdateSoulChunks(float deltaTime)
        {
            for (auto it = m_SoulChunks.begin(); it < m_SoulChunks.end(); it++)
            {
                if ((*it)->WasCollected())
                {
                    m_MapGrid.RemoveObjectOnGrid(*it);
                    it = m_SoulChunks.erase(it);

                    if (m_SoulChunks.size() == 0)
                    {
                        m_Door->OpenDoor();
                    }

                    if (it == m_SoulChunks.end()) { break; }
                }
                else 
                {
                    (*it)->Update(deltaTime);
                }
            }
        }

        void GameMap::UpdateMovingPlatforms(float deltaTime) 
        {
            for (const std::shared_ptr<MovingPlatform>& platform : m_MovingPlatforms)
            {
                platform->Update(deltaTime);
            }
        }

        bool GameMap::LoadTileMap(const std::vector<int>& tiles, const sf::Vector2u& levelSize)
        {
            // Resize the vertex array to fit the level size
            m_BackgroundTileMap.clear();
            m_BackgroundTileMap.setPrimitiveType(sf::Quads);
            m_BackgroundTileMap.resize(static_cast<size_t>(levelSize.x) * static_cast<size_t>(levelSize.y) * 4);
    
            // Clear the map grid
            m_MapGrid.Clear();

            // Populate the vertex array, with one quad per tile
            for (unsigned int i = 0; i < levelSize.x; ++i) 
            {
                std::vector<std::shared_ptr<Engine::BoxCollideable>> tileLine;

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

                m_MapGrid.AddTileLineOnGrid(tileLine);
            }

            return true;
        }

        void GameMap::CreateVertexQuad(unsigned int i, unsigned int j, const sf::Vector2u& levelSize, int tu, int tv)
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

        void GameMap::CreateTile(int tileNumber, std::vector<std::shared_ptr<Engine::BoxCollideable>>& tileLine, unsigned int i, unsigned int j, const sf::Vector2u& levelSize)
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

                case TileType::LADDER:
                {
                    tileLine.push_back(std::make_shared<ClimbableTile>
                        (static_cast<float>(xCenter)
                            , static_cast<float>(yCenter)
                            , static_cast<float>(TILE_SIZE.x)
                            , static_cast<float>(TILE_SIZE.y)
                            ));
                    break;
                }

                case TileType::LAVA:
                {
                    tileLine.push_back(std::make_shared<DeadlyTile>
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

        void GameMap::InitObjectsAndEntities(const std::map<std::string, std::vector<std::string>>& configKeymap, bool restart)
        {
            InitPlayer(configKeymap, restart);
            InitEnemies(configKeymap);
            InitSoulChunks(configKeymap);
            InitMovingPlatforms(configKeymap);
            InitDoor(configKeymap);
        }

        void GameMap::InitPlayer(const std::map<std::string, std::vector<std::string>>& configKeymap, bool restart)
        {
            const std::vector<std::string> playerInfo = configKeymap.at("PLAYER_POSITION");

            sf::Vector2f playerPosition
            {
                std::stof(playerInfo.at(0)) * TILE_SIZE.x + (TILE_SIZE.x / 2),
                std::stof(playerInfo.at(1)) * TILE_SIZE.y + (TILE_SIZE.y / 2)
            };

            m_Player.Reset(playerPosition, restart);
        }

        void GameMap::InitEnemies(const std::map<std::string, std::vector<std::string>>& configKeymap) 
        {
            m_Enemies.clear();

            const int nbEnemies = std::stoi(configKeymap.at("NUMBER_ENEMIES").at(0));
            m_Enemies.reserve(nbEnemies);

            for (int i = 0; i < nbEnemies; i++)
            {
                const std::vector<std::string> enemyInfo = configKeymap.at("ENEMY_" + std::to_string(i));

                sf::Vector2f enemyPosition
                {
                    std::stof(enemyInfo.at(0)) * TILE_SIZE.x + (TILE_SIZE.x / 2),
                    std::stof(enemyInfo.at(1)) * TILE_SIZE.y + (TILE_SIZE.y / 2)
                };

                std::shared_ptr<Enemy>& enemy = m_Enemies.emplace_back(std::make_shared<Enemy>(m_TextureManager, enemyPosition));
                m_MapGrid.AddObjectOnGrid(enemy);
            }
        }

        void GameMap::InitSoulChunks(const std::map<std::string, std::vector<std::string>>& configKeymap) 
        {
            m_SoulChunks.clear();

            const int nbSoulChunks = std::stoi(configKeymap.at("NUMBER_SOULCHUNKS").at(0));
            m_SoulChunks.reserve(nbSoulChunks);

            for (int i = 0; i < nbSoulChunks; i++)
            {
                const std::vector<std::string> soulChunkInfo = configKeymap.at("SOULCHUNK_" + std::to_string(i));

                sf::Vector2f soulChunkPosition
                {
                    std::stof(soulChunkInfo.at(0)) * TILE_SIZE.x + (TILE_SIZE.x / 2),
                    std::stof(soulChunkInfo.at(1)) * TILE_SIZE.y + (TILE_SIZE.y / 2)
                };

                std::shared_ptr<SoulChunk>& soulChunk = m_SoulChunks.emplace_back(std::make_shared<SoulChunk>(m_TextureManager, soulChunkPosition));
                m_MapGrid.AddObjectOnGrid(soulChunk);
            }
        }

        void GameMap::InitMovingPlatforms(const std::map<std::string, std::vector<std::string>>& configKeymap)
        {
            m_MovingPlatforms.clear();

            const int nbMovingPlatforms = std::stoi(configKeymap.at("NUMBER_MOVING_PLATFORMS").at(0));
            m_MovingPlatforms.reserve(nbMovingPlatforms);

            for (int i = 0; i < nbMovingPlatforms; i++)
            {
                const std::vector<std::string> platformInfo = configKeymap.at("MOVING_PLATFORM_" + std::to_string(i));
        
                sf::Vector2f platformStartPosition
                {
                    std::stof(platformInfo.at(0)) * TILE_SIZE.x,
                    std::stof(platformInfo.at(1)) * TILE_SIZE.y
                };

                sf::Vector2f platformEndPosition
                {
                    std::stof(platformInfo.at(2)) * TILE_SIZE.x,
                    std::stof(platformInfo.at(3)) * TILE_SIZE.y
                };

                sf::Vector2f platformSize
                {
                    std::stof(platformInfo.at(4)),
                    std::stof(platformInfo.at(5))
                };
        
                std::shared_ptr<MovingPlatform>& platform = m_MovingPlatforms.emplace_back(std::make_shared<MovingPlatform>(platformStartPosition, platformEndPosition, platformSize));
                m_MapGrid.AddObjectOnGrid(platform);
            }
        }

        void GameMap::InitDoor(const std::map<std::string, std::vector<std::string>>& configKeymap)
        {
            const std::vector<std::string> doorInfo = configKeymap.at("DOOR");

            sf::Vector2f doorPosition
            {
                std::stof(doorInfo.at(0)) * TILE_SIZE.x + (TILE_SIZE.x / 2),
                std::stof(doorInfo.at(1)) * TILE_SIZE.y + (TILE_SIZE.y / 2)
            };

            sf::Vector2f doorSize
            {
                std::stof(doorInfo.at(2)),
                std::stof(doorInfo.at(3))
            };

            m_Door = std::make_shared<Door>(doorPosition, doorSize);
            m_MapGrid.AddObjectOnGrid(m_Door);
        }
    }
}