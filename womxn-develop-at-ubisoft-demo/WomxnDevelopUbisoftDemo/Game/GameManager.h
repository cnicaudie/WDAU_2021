#pragma once

#include <Game/Objects/Bullet.h>
#include <Game/Objects/Door.h>
#include "Player.h"
#include "Enemy.h"
#include "LevelManager.h"
#include <UI/UIManager.h>
#include <Engine/Collision/CollisionManager.h>

class GameManager : public Game
{
public:
    static GameManager* GetInstance();
    GameManager(const GameManager& gameManager) = delete;
    void operator=(const GameManager& gameManager) = delete;

    void Update(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;

    inline bool CheckPlayerMovementX(const sf::Vector2f& nextPosition) 
    {
        return m_CollisionManager.CheckPlayerCollisionX(m_Player, nextPosition, m_LevelManager.GetMap());
    }

    inline bool CheckPlayerMovementY(const sf::Vector2f& nextPosition)
    {
        return m_CollisionManager.CheckPlayerCollisionY(m_Player, nextPosition, m_LevelManager.GetMap());
    }

    inline bool CheckBulletImpact(const Bullet& bullet) 
    {
        return m_CollisionManager.CheckBulletCollisionWithEnemies(bullet, m_Enemies) 
            || m_CollisionManager.CheckBulletCollisionWithMap(bullet, m_LevelManager.GetMap());
    }

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