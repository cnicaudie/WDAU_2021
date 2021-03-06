#pragma once

#include <Engine/Collision/CollisionManager.h>
#include <UI/UIManager.h>
#include <Game/Objects/Bullet.h>
#include <Game/Objects/Door.h>
#include <Game/Entities/Player.h>
#include <Game/Entities/Enemy.h>
#include "LevelManager.h"

class GameManager : public Game
{
public:
    static GameManager* GetInstance();
    GameManager(const GameManager& gameManager) = delete;
    void operator=(const GameManager& gameManager) = delete;

    void Update(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;

    inline bool CheckPlayerMovement(const sf::Vector2f& positionOffset)
    {
        return m_CollisionManager.CheckCollision(&m_Player, positionOffset, m_LevelManager.GetMap().GetMapGrid());
    }

    inline sf::Vector2u GetLevelBounds() { return m_LevelManager.GetLevelBounds(); };

    inline bool CheckBulletImpact(Bullet* bullet, const sf::Vector2f& positionOffset)
    {
        return m_CollisionManager.CheckCollision(bullet, positionOffset, m_LevelManager.GetMap().GetMapGrid());
    }

    /*
    inline bool CheckPlayerCollectedSoulChunk(const SoulChunk& soulChunk)
    {
        return m_CollisionManager.CheckPlayerTriggerWithSoulChunk(m_Player, soulChunk);
    }
    */

private:
    GameManager();
    ~GameManager();

    static GameManager* m_GameManager; // Singleton instance

    std::shared_ptr<TextureManager> m_TextureManager;
    UIManager m_UiManager;
    LevelManager m_LevelManager;
    
    CollisionManager m_CollisionManager;
    Player m_Player;
    std::vector<Enemy> m_Enemies;

    Door m_Door;
    
    sf::View m_CameraView;
    
    bool m_IsGameOver;
};