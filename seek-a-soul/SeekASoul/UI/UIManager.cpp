#include <stdafx.h>
#include "UIManager.h"
#include <sstream>

UIManager::UIManager(sf::RenderWindow* window)
    : m_Window(window)
    , m_GUIView(sf::FloatRect(0.f, 0.f, static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)))
    , m_IsPlayingEndGame(false)
{
    const sf::Vector2f WINDOW_CENTER{ m_GUIView.getCenter() };

    // TODO : Move in a sound manager
    m_EndgameSoundBuffer.loadFromFile("Assets\\Sounds\\Test.wav");
    m_EndgameSound.setBuffer(m_EndgameSoundBuffer);

    m_MainFont.loadFromFile("Assets\\Fonts\\arial.ttf");
    m_EndgameText.setFont(m_MainFont);
    m_AmmunitionsText.setFont(m_MainFont);

    m_EndgameText.setCharacterSize(50);
    m_AmmunitionsText.setCharacterSize(25);

    m_EndgameText.setFillColor(sf::Color::Green);
    m_AmmunitionsText.setFillColor(sf::Color::Red);

    m_EndgameText.setString("!!! WIN !!!");
    m_EndgameText.setStyle(sf::Text::Bold);

    float textWidth = m_EndgameText.getGlobalBounds().width;
    float textHeight = m_EndgameText.getGlobalBounds().height;
    
    m_EndgameText.setPosition(WINDOW_CENTER.x - (textWidth / 2), WINDOW_CENTER.y - (textHeight / 2));
    m_AmmunitionsText.setPosition(WINDOW_CENTER.x * 1.6f, WINDOW_CENTER.y * 0.05f);

    EventListener<UIManager, Event> listener(this, &UIManager::OnEvent);
    EventManager::GetInstance()->AddListener(listener);
}

UIManager::~UIManager() 
{
    EventListener<UIManager, Event> listener(this, &UIManager::OnEvent);
    EventManager::GetInstance()->RemoveListener(listener);
}

void UIManager::Update(float deltaTime)
{
    std::stringstream stream;
    stream << "Bullets left : " << UIViewModel::GetInstance()->GetAmmunitionsNumber();
    m_AmmunitionsText.setString(stream.str());

    m_Window->setView(m_GUIView);
}

void UIManager::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    if (m_IsPlayingEndGame) 
    {
        target.draw(m_EndgameText);
    } 
    else 
    {
        target.draw(m_AmmunitionsText);
    }
}

void UIManager::OnEvent(const Event* evnt)
{
    // dynamic_cast if necessary

    if (evnt->GetEventType() == EventType::END_GAME)
    {
        StartEndGame();
    }
}

void UIManager::StartEndGame() 
{
    m_IsPlayingEndGame = true;
    m_EndgameSound.play();
}
