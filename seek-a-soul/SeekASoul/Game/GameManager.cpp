#include <stdafx.h>
#include "GameManager.h"


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
    , m_UiManager{}
    , m_LevelManager { m_InputManager, m_TextureManager }
    , m_CameraManager{ &m_Window, &(m_LevelManager.GetPlayerOnMap()) }
    , m_IsGameOver{ false }
{
    EventHandler<GameManager>* handler = new EventHandler<GameManager>(this, &GameManager::StartEndGame);
    EventManager::GetInstance()->AddListener(Event(EventType::GAME_OVER), handler);
}

GameManager::~GameManager()
{
    delete m_GameManager;
}

void GameManager::Update(float deltaTime)
{
    m_InputManager->UpdateMousePosition(m_Window);
    
    if (!m_IsGameOver)
    {
        m_LevelManager.Update(deltaTime);
        m_CameraManager.Update(deltaTime);
    }

    EventManager::GetInstance()->Update(deltaTime);
}

void GameManager::Render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));
    target.draw(m_LevelManager);
    target.draw(m_UiManager);
    target.draw(m_CameraManager);
}

void GameManager::RenderDebugMenu(sf::RenderTarget& target)
{
    ImGui::Begin("Debug Menu");
    ImGui::Text("Press F1 to close this debug menu");
    ImGui::NewLine();
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

void GameManager::StartEndGame()
{
    std::cout << "GameManager end game" << std::endl;
    m_IsGameOver = true;
}