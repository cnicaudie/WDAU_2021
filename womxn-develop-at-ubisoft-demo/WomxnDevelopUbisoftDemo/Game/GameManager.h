#pragma once

#include "Bullet.h"
#include "Door.h"
#include "Player.h"
#include "Wall.h"
#include "Enemy.h"
#include "LevelManager.h"
#include <UI/UIManager.h>

class GameManager : public Game
{
public:
    GameManager();

    void Update(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;

private:
    std::shared_ptr<TextureManager> m_TextureManager;
    UIManager m_UiManager;
    LevelManager m_LevelManager;
    
    Player m_Player;
    Enemy m_Enemy;

    Door m_Door;
    // For test purposes
    Wall m_Ground;
    Wall m_Wall;
    Wall m_Platform;

    sf::View m_CameraView;
    
    bool m_IsGameOver;
};