#pragma once

#include <Engine/Game.h>
#include "Door.h"
#include "MainCharacter.h"
#include "Wall.h"
#include "Enemy.h"

class GameDemo : public Game
{
public:
    GameDemo();

    void Update(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;

private:
    sf::Font m_EndgameTextFont;
    sf::Text m_EndgameText;
    sf::SoundBuffer m_EndgameSoundBuffer;
    sf::Sound m_EndgameSound;
    MainCharacter m_MainCharacter;
    Enemy m_Enemy;
    Door m_Door;

    // For test purposes
    Wall m_Ground;
    Wall m_Wall;
    Wall m_Platform;

    // View
    sf::View m_cameraView;

    bool m_IsFinished;
};