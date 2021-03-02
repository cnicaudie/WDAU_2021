#pragma once

#include "Bullet.h"
#include "Door.h"
#include "Player.h"
#include "Wall.h"
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

    bool CheckPlayerMovementX(const sf::Vector2f& nextPosition) 
    {
        return m_CollisionManager.CheckCollisionX(m_Player, nextPosition, m_LevelManager.GetMap());
    }

    bool CheckPlayerMovementY(const sf::Vector2f& nextPosition)
    {
        return m_CollisionManager.CheckCollisionY(m_Player, nextPosition, m_LevelManager.GetMap());
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
    Enemy m_Enemy;

    Door m_Door;
    
    sf::View m_CameraView;
    
    bool m_IsGameOver;
};