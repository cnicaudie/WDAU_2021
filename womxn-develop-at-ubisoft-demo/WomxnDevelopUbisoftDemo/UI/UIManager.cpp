#include <stdafx.h>
#include "UIManager.h"


UIManager::UIManager() 
    : m_IsPlayingEndGame(false)
{
    m_EndgameTextFont.loadFromFile("Assets\\arial.ttf");

    m_EndgameText.setFont(m_EndgameTextFont);
    m_EndgameText.setPosition(500, 400);
    m_EndgameText.setString("!!! WIN !!!");
    m_EndgameText.setCharacterSize(24);
    m_EndgameText.setFillColor(sf::Color::Red);

    m_EndgameSoundBuffer.loadFromFile("Assets\\Test.wav");

    m_EndgameSound.setBuffer(m_EndgameSoundBuffer);
}

void UIManager::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    if (m_IsPlayingEndGame) 
    {
        target.draw(m_EndgameText);
    }
}

void UIManager::StartEndGame() 
{
    m_IsPlayingEndGame = true;
    m_EndgameSound.play();
}
