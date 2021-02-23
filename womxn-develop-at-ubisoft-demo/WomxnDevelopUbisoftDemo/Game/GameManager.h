#pragma once

#include "Bullet.h"
#include "Door.h"
#include "Player.h"
#include "Wall.h"
#include "Enemy.h"
#include <UI/UIManager.h>

class GameManager : public Game
{
public:
    GameManager();

    void Update(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;

private:
    bool m_IsGameOver;

    // Player
    Player m_Player;

    // Enemies
    Enemy m_Enemy;

    // Map elements
    Door m_Door;

    // For test purposes
    Wall m_Ground;
    Wall m_Wall;
    Wall m_Platform;

    // View
    sf::View m_CameraView;

    // UI Elements
    UIManager m_UiManager;
};