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
    : Game{ "Game Demo" }
    , m_TextureManager{ std::make_shared<TextureManager>() }
    , m_UiManager{}
    , m_LevelManager { m_TextureManager }
    , m_Player{ m_InputManager, m_TextureManager }
    , m_CameraView{}
    , m_IsGameOver{ false }
{
    // TODO : Make a camera manager
    // Camera setup
    sf::Vector2f viewSize{ 1024, 768 };
    m_CameraView.setSize(viewSize);
    //m_CameraView.zoom(1.5f);
    m_CameraView.setCenter(m_Player.GetCenter());
    //view.setViewport(sf::FloatRect(0.0f, 0.0f, 0.5f, 1.0f));
    m_Window.setView(m_CameraView);

    // TODO : Move enemies in Map class
    m_Enemies.emplace_back(m_TextureManager);
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
        m_Player.Update(deltaTime);
        m_LevelManager.Update(deltaTime);

        for (Enemy& enemy : m_Enemies)
        {
            enemy.Update(deltaTime);
        }
    
        // Move the camera view according to the player's position
        m_CameraView.setCenter(m_Player.GetCenter());
        m_Window.setView(m_CameraView);
    }
}

void GameManager::Render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));
    target.draw(m_LevelManager);
    target.draw(m_Player);

    for (const Enemy& enemy : m_Enemies)
    {
        target.draw(enemy);
    }

    target.draw(m_UiManager);
}

void GameManager::RenderDebugMenu(sf::RenderTarget& target)
{
    ImGui::Begin("Debug Menu");
    ImGui::Text("Press F1 to close this debug menu");
    ImGui::NewLine();

    if (ImGui::CollapsingHeader("Main character infos"))
    {
        const auto& mainCharCenterPos = m_Player.GetCenter();
        const auto& mainCharVelocity = m_Player.GetVelocity();

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