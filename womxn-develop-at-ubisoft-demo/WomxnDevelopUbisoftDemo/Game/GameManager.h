#pragma once

#include <Engine/Collision/CollisionManager.h>
#include <UI/UIManager.h>
#include <Game/Entities/Player.h>
#include "LevelManager.h"
#include <Game/Camera/CameraManager.h>

class GameManager : public Game
{
public:
    static GameManager* GetInstance();
    GameManager(const GameManager& gameManager) = delete;
    void operator=(const GameManager& gameManager) = delete;

    void Update(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;

    inline sf::Vector2u GetLevelBounds() { return m_LevelManager.GetLevelBounds(); };

    inline bool CheckCollision(BoxCollideable* collideable, const sf::Vector2f& positionOffset)
    {
        return m_CollisionManager.CheckCollision(collideable, positionOffset, m_LevelManager.GetMap().GetMapGrid());
    }

    inline void StartEndGame()
    {
        m_UiManager.StartEndGame();
        m_IsGameOver = true;
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
    CameraManager m_CameraManager;
    
    bool m_IsGameOver;
};