#include <stdafx.h>
#include "GameManager.h"
#include <Engine/Event/Listener/EventListener.h>

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
    , m_UIManager{ &m_Window }
    , m_LevelManager { m_InputManager, m_TextureManager }
    , m_CameraManager{ &m_Window }
    , m_IsGameOver{ false }
    , m_FramesPerSecond(60)
{   
    m_CameraManager.SetBoxToFollow(&(m_LevelManager.GetPlayerOnMap()));

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
    
    if (!m_IsGameOver)
    {
        m_InputManager->Update();
        m_LevelManager.Update(deltaTime);
        m_CameraManager.Update(deltaTime);
    }
}

void GameManager::UpdateGUI(float deltaTime)
{
    m_UIManager.Update(deltaTime);
}

void GameManager::Render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));
    target.draw(m_LevelManager);
    target.draw(m_CameraManager);
}

void GameManager::RenderGUI(sf::RenderTarget& target) 
{
    target.draw(m_UIManager);
}

void GameManager::RenderDebugMenu(sf::RenderTarget& target)
{
    ImGui::Begin("Debug Menu");
    ImGui::Text("Press F1 to close this debug menu");
    ImGui::NewLine();
    ImGui::Text("FPS : %d", m_FramesPerSecond);
    ImGui::Checkbox("Show Camera Zones", &m_CameraManager.DisplayCameraZones);

    if (ImGui::CollapsingHeader("Main character infos"))
    {
        const Player& player = m_LevelManager.GetPlayerOnMap();
        const sf::Vector2f& mainCharCenterPos = player.GetCenter();
        const sf::Vector2f& mainCharVelocity = player.GetVelocity();

        ImGui::Text("Position infos :");
        ImGui::Text("X: %f", mainCharCenterPos.x);
        ImGui::Text("Y: %f", mainCharCenterPos.y);

        ImGui::Text("Velocity infos :");
        ImGui::Text("X: %f", mainCharVelocity.x);
        ImGui::Text("Y: %f", mainCharVelocity.y);
    }

    if (ImGui::CollapsingHeader("Mouse position"))
    {
        const sf::Vector2f& mouseWorldPosition = m_InputManager->GetMousePosition();

        ImGui::Text("X: %f", mouseWorldPosition.x);
        ImGui::Text("Y: %f", mouseWorldPosition.y);
    }

    if (ImGui::CollapsingHeader("Game status"))
    {
        if (m_IsGameOver)
        {
            ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "GAME ENDED");
        }
        else
        {
            ImGui::TextColored(ImVec4(0.f, 255.0f, 0.f, 1.f), "GAME IN PROGRESS");
        }
    }

    ImGui::End();
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