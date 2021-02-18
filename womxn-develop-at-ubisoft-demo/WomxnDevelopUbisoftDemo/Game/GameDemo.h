#pragma once

#include <Engine/Game.h>
#include <Game/Door.h>
#include <Game/MainCharacter.h>
#include <Game/Wall.h>

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
    Door m_Door;
    Wall m_Wall;
    bool m_IsFinished;
};