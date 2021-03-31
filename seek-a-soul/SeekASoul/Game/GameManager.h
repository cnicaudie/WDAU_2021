#pragma once

#include <Engine/Collision/CollisionManager.h>
#include <Game/Camera/CameraManager.h>
#include <UI/UIManager.h>
#include "LevelManager.h"

class GameManager : public Game
{
public:
    static GameManager* GetInstance();
    GameManager(const GameManager& gameManager) = delete;
    void operator=(const GameManager& gameManager) = delete;

    void Update(float deltaTime) override;
    void UpdateGUI(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderGUI(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;

    inline sf::Vector2u GetLevelBounds() { return m_LevelManager.GetLevelBounds(); };
    
    inline bool CheckCollision(BoxCollideable* collideable, const sf::Vector2f& positionOffset)
    {
        return m_CollisionManager.CheckCollision(collideable, positionOffset, m_LevelManager.GetMap().GetMapGrid());
    }

private:
    GameManager();
    ~GameManager();

    void OnEvent(const Event* evnt);
    void StartEndGame();

    //====================//

    static GameManager* m_GameManager; // Singleton instance

    std::shared_ptr<TextureManager> m_TextureManager;
    UIManager m_UIManager;
    LevelManager m_LevelManager;
    CameraManager m_CameraManager;
    CollisionManager m_CollisionManager;
    
    bool m_IsGameOver;

    sf::Clock m_FPSUpdateClock;
    int m_FramesPerSecond; // For Debug purposes
};