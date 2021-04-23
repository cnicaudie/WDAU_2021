#include <stdafx.h>
#include "GameManager.h"
#include <Engine/Event/Listener/EventListener.h>
#include <Engine/Event/EventTypes/LevelEvent.h>
#include <Engine/Collision/CollisionManager.h>
#include <Game/Camera/CameraManager.h>
#include <Game/Level/LevelManager.h>
#include <UI/UIManager.h>

GameManager* GameManager::m_GameManager = nullptr;

GameManager* GameManager::GetInstance() 
{
    if (m_GameManager == nullptr) 
    {
        m_GameManager = new GameManager();
    }

    return m_GameManager;
}

GameManager::GameManager()
    : Game{ "Seek A Soul (WIP)" }
    , m_TextureManager{ std::make_shared<TextureManager>() }
    , m_UIManager{ std::make_unique<UIManager>(&m_Window) }
    , m_LevelManager { std::make_unique<LevelManager>(m_InputManager, m_TextureManager) }
    , m_CameraManager{ std::make_unique<CameraManager>(&m_Window) }
    , m_CurrentState(GameState::NOT_STARTED)
    , m_FramesPerSecond(60)
{   
    m_CameraManager->SetBoxToFollow(&(m_LevelManager->GetPlayerOnMap()));

    EventListener<GameManager, Event> listenerEvent(this, &GameManager::OnEvent);
    EventListener<GameManager, LevelEvent> listenerLevelEvent(this, &GameManager::OnEvent);
    EventManager::GetInstance()->AddListener(listenerEvent);
    EventManager::GetInstance()->AddListener(listenerLevelEvent);
}

GameManager::~GameManager()
{
    EventListener<GameManager, Event> listenerEvent(this, &GameManager::OnEvent);
    EventListener<GameManager, LevelEvent> listenerLevelEvent(this, &GameManager::OnEvent);
    EventManager::GetInstance()->RemoveListener(listenerEvent);
    EventManager::GetInstance()->RemoveListener(listenerLevelEvent);

    delete m_GameManager;
}

void GameManager::Update(float deltaTime)
{
    if (m_FPSUpdateClock.getElapsedTime().asSeconds() >= 1.f) 
    {
        m_FramesPerSecond = static_cast<int>(1.f / deltaTime);
        m_FPSUpdateClock.restart();
    }

    EventManager::GetInstance()->Update();
    m_LevelManager->Update(deltaTime);
    
    if (m_CurrentState != GameState::NOT_STARTED)
    {
        m_CameraManager->Update(deltaTime);

        if (m_CurrentState == GameState::PLAYING)
        {
            m_InputManager->UpdateMousePosition(m_Window, true);
            m_InputManager->Update();
        }
    }
}

void GameManager::UpdateGUI(float deltaTime)
{
    m_UIManager->Update(deltaTime);
    m_InputManager->UpdateMousePosition(m_Window, false);
}

void GameManager::Render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));

    if (m_CurrentState != GameState::NOT_STARTED)
    {
        target.draw(*m_LevelManager);
        target.draw(*m_CameraManager);
    }
}

void GameManager::RenderGUI(sf::RenderTarget& target) 
{
    target.draw(*m_UIManager);
}

void GameManager::RenderDebugMenu(sf::RenderTarget& target)
{
    ImGui::SetNextWindowSize(ImVec2(250.f, 250.f), ImGuiCond_FirstUseEver);
    ImGui::Begin("Debug Menu (Press F1 to close)");
    ImGui::Text("FPS : %d", m_FramesPerSecond);
    ImGui::Text("Game Status : ");
    ImGui::SameLine();

    switch(m_CurrentState) 
    {
        case GameState::NOT_STARTED: 
        { 
            ImGui::TextColored(ImVec4(0.f, 255.0f, 255.0f, 1.f), "NOT STARTED");
            break;
        }

        case GameState::PLAYING:
        {
            ImGui::TextColored(ImVec4(0.f, 255.0f, 0.f, 1.f), "IN PROGRESS");
            break;
        }

        case GameState::OVER:
        {
            ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "ENDED");
            break;
        }

        default:
            break;
    }

    if (ImGui::Button("End Game")) 
    {
        std::shared_ptr<Event> eventType = std::make_shared<Event>(EventType::END_GAME);
        EventManager::GetInstance()->Fire(eventType);
    }

    m_LevelManager->RenderDebugMenu(target);
    m_CameraManager->RenderDebugMenu(target);
    m_InputManager->RenderDebugMenu(target);
    ImGui::End();
}

const sf::Vector2u GameManager::GetLevelBounds() const 
{ 
    return m_LevelManager->GetLevelBounds(); 
}

const bool GameManager::CheckCollision(BoxCollideable* collideable, const sf::Vector2f& positionOffset) const
{
    return m_CollisionManager->CheckCollision(collideable, positionOffset, m_LevelManager->GetMap().GetMapGrid());
}

void GameManager::OnEvent(const Event* evnt)
{
    if (evnt->GetEventType() == EventType::START_GAME)
    {
        m_CurrentState = GameState::PLAYING;
    }
    else if (evnt->GetEventType() == EventType::END_GAME)
    {
        LOG_INFO("GAME OVER !!!");
        m_CurrentState = GameState::OVER;
    }
    else if (const LevelEvent* actionEvent = dynamic_cast<const LevelEvent*>(evnt))
    {
        if (actionEvent->GetLevelStatus() == LevelStatus::RESTART)
        {
            m_CurrentState = GameState::PLAYING;
        }
    }
}