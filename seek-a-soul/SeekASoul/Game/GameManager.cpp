#include <stdafx.h>
#include "GameManager.h"
#include <Engine/Event/Listener/EventListener.h>
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
    , m_IsGameOver{ false }
    , m_FramesPerSecond(60)
{   
    m_CameraManager->SetBoxToFollow(&(m_LevelManager->GetPlayerOnMap()));

    EventListener<GameManager, Event> listener(this, &GameManager::OnEvent);
    EventManager::GetInstance()->AddListener(listener);
}

GameManager::~GameManager()
{
    EventListener<GameManager, Event> listener(this, &GameManager::OnEvent);
    EventManager::GetInstance()->RemoveListener(listener);

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
    
    m_CameraManager->Update(deltaTime);
    m_LevelManager->Update(deltaTime);

    if (!m_IsGameOver)
    {
        m_InputManager->UpdateMousePosition(m_Window, true);
        m_InputManager->Update();
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
    target.draw(*m_LevelManager);
    target.draw(*m_CameraManager);
}

void GameManager::RenderGUI(sf::RenderTarget& target) 
{
    target.draw(*m_UIManager);
}

void GameManager::RenderDebugMenu(sf::RenderTarget& target)
{
    ImGui::Begin("Debug Menu (Press F1 to close)");
    ImGui::Text("FPS : %d", m_FramesPerSecond);
    ImGui::Text("Game Status : ");
    ImGui::SameLine();
    m_IsGameOver 
        ? ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "GAME ENDED") 
        : ImGui::TextColored(ImVec4(0.f, 255.0f, 0.f, 1.f), "GAME IN PROGRESS");

    if (ImGui::Button("End Game")) 
    {
        std::shared_ptr<Event> eventType = std::make_shared<Event>(EventType::END_GAME);
        EventManager::GetInstance()->Fire(eventType);
    }

    m_CameraManager->RenderDebugMenu(target);
    m_LevelManager->RenderDebugMenu(target);
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
    // dynamic_cast if necessary
    
    if (evnt->GetEventType() == EventType::END_GAME) 
    {
        StartEndGame();
    }
}

void GameManager::StartEndGame()
{
    LOG_INFO("GAME OVER !!!");
    m_IsGameOver = true;
}