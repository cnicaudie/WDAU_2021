#pragma once

#include <Engine/Collision/CollisionManager.h>
#include <UI/UIManager.h>
#include <Game/Entities/Player.h>
#include "LevelManager.h"
#include <Game/Camera/CameraManager.h>
#include <Engine/Event/EventHandler.h>

class GameManager : public Game
{
public:
    static GameManager* GetInstance();
    GameManager(const GameManager& gameManager) = delete;
    void operator=(const GameManager& gameManager) = delete;

    void Update(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;
    void StartEndGame();

    inline sf::Vector2u GetLevelBounds() { return m_LevelManager.GetLevelBounds(); };
    
    inline bool CheckCollision(BoxCollideable* collideable, const sf::Vector2f& positionOffset)
    {
        return m_CollisionManager.CheckCollision(collideable, positionOffset, m_LevelManager.GetMap().GetMapGrid());
    }

private:
    GameManager();
    ~GameManager();

    //====================//

    static GameManager* m_GameManager; // Singleton instance

    std::shared_ptr<TextureManager> m_TextureManager;
    UIManager m_UiManager;
    LevelManager m_LevelManager;
    CameraManager m_CameraManager;
    CollisionManager m_CollisionManager;
    
    bool m_IsGameOver;
};