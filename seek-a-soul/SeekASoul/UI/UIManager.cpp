#include <stdafx.h>
#include "UIManager.h"
#include <sstream>
#include <Engine/Event/EventTypes/LevelEvent.h>

UIManager::UIManager(sf::RenderWindow* window)
    : m_Window(window)
    , m_GUIView(sf::FloatRect(0.f, 0.f, static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)))
    , m_IsPlayingEndGame(false)
    , m_Button(m_GUIView.getCenter() * 1.5f, { 150.f, 50.f })
{
    const sf::Vector2f WINDOW_CENTER{ m_GUIView.getCenter() };

    // TODO : Move in a sound manager
    m_EndgameSoundBuffer.loadFromFile("Assets\\Sounds\\Test.wav");
    m_EndgameSound.setBuffer(m_EndgameSoundBuffer);

    m_MainFont.loadFromFile("Assets\\Fonts\\arial.ttf");
    
    // Configure Buttons
    m_Button.SetButtonTextFont(m_MainFont);
    m_Button.SetButtonTextString("Restart");
    m_Button.SetButtonTextPosition(WINDOW_CENTER * 1.5f);

    // Configure Texts
    m_EndgameText.setFont(m_MainFont);
    m_EndgameText.setCharacterSize(50);
    m_EndgameText.setFillColor(sf::Color::Green);
    m_EndgameText.setString("!!! WIN !!!");
    m_EndgameText.setStyle(sf::Text::Bold);
    float textWidth = m_EndgameText.getGlobalBounds().width;
    float textHeight = m_EndgameText.getGlobalBounds().height;
    m_EndgameText.setPosition(WINDOW_CENTER.x - (textWidth / 2), WINDOW_CENTER.y - (textHeight / 2));
    
    m_AmmunitionsText.setFont(m_MainFont);
    m_AmmunitionsText.setCharacterSize(25);
    m_AmmunitionsText.setFillColor(sf::Color::Red);
    m_AmmunitionsText.setPosition(WINDOW_CENTER.x * 1.6f, WINDOW_CENTER.y * 0.05f);
    
    m_SoulChunksText.setFont(m_MainFont);
    m_SoulChunksText.setCharacterSize(25);
    m_SoulChunksText.setFillColor(sf::Color::Cyan);
    m_SoulChunksText.setPosition(WINDOW_CENTER.x * 1.6f, WINDOW_CENTER.y * 0.15f);

    // Configure EventListeners
    EventListener<UIManager, Event> listenerGameOver(this, &UIManager::OnEvent);
    EventListener<UIManager, LevelEvent> listenerLevelEnd(this, &UIManager::OnEvent);
    EventManager::GetInstance()->AddListener(listenerGameOver);
    EventManager::GetInstance()->AddListener(listenerLevelEnd);
}

UIManager::~UIManager() 
{
    // Remove listeners
    EventListener<UIManager, Event> listenerGameOver(this, &UIManager::OnEvent);
    EventListener<UIManager, LevelEvent> listenerLevelEnd(this, &UIManager::OnEvent);
    EventManager::GetInstance()->RemoveListener(listenerGameOver);
    EventManager::GetInstance()->RemoveListener(listenerLevelEnd);
}

void UIManager::Update(float deltaTime)
{
    // Update texts
    std::stringstream stream;
    stream << "Bullets left : " << UIViewModel::GetInstance()->GetAmmunitionsNumber();
    m_AmmunitionsText.setString(stream.str());
    
    // Clear the stream content
    stream.str(std::string()); // resets the string content
    stream.clear(); // clears fails and eof flags

    stream << "Soul chunks : " << UIViewModel::GetInstance()->GetSoulChunksNumber();
    m_SoulChunksText.setString(stream.str());

    // Update the view
    m_Window->setView(m_GUIView);

    // Manage buttons
    if (m_IsPlayingEndGame && m_Button.WasClicked())
    {
        LOG_DEBUG("Button was clicked !");
        m_Button.ResetClickStatus();
        m_IsPlayingEndGame = false;

        std::shared_ptr<LevelEvent> levelEvent = std::make_shared<LevelEvent>(LevelStatus::RESTART);
        EventManager::GetInstance()->Fire(levelEvent);
    }
}

void UIManager::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    target.draw(m_AmmunitionsText);
    target.draw(m_SoulChunksText);

    if (m_IsPlayingEndGame) 
    {
        target.draw(m_Button);
        target.draw(m_EndgameText);
    }
}

void UIManager::OnEvent(const Event* evnt)
{
    if (evnt->GetEventType() == EventType::END_GAME)
    {
        StartEndGame();
    }

    if (const LevelEvent* actionEvent = dynamic_cast<const LevelEvent*>(evnt))
    {
        if (actionEvent->GetLevelStatus() == LevelStatus::FAILURE)
        {
            m_EndgameText.setFillColor(sf::Color::Red);
            m_EndgameText.setString("YOU LOST...");
        }
    }
}

void UIManager::StartEndGame() 
{
    m_IsPlayingEndGame = true;
    m_EndgameSound.play();
}
