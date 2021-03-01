#include <stdafx.h>
#include "GameManager.h"

GameManager::GameManager()
    : Game{ "Game Demo" }
    , m_TextureManager{ std::make_shared<TextureManager>() }
    , m_UiManager{}
    , m_LevelManager { m_TextureManager }
    , m_Player{ m_InputManager, m_TextureManager }
    , m_Enemy{ m_TextureManager }
    , m_Door{ 900, 600, 100, 200 }
    , m_Ground{ 400, 700, 500, 25 }
    , m_Wall{ 200, 500, 25, 200 }
    , m_Platform{ 400, 500, 100, 25 }
    , m_CameraView{}
    , m_IsGameOver{ false }
{
    std::vector<Wall> walls{ m_Wall, m_Ground, m_Platform };
    m_Player.InitColliders(walls);

    sf::Vector2f viewSize{ 1024, 768 };
    m_CameraView.setSize(viewSize);
    m_CameraView.zoom(1.5f);
    m_CameraView.setCenter(m_Player.GetCenter());
    //view.setViewport(sf::FloatRect(0.0f, 0.0f, 0.5f, 1.0f));
    m_Window.setView(m_CameraView);

    m_LevelManager.LoadLevel(0);
}

void GameManager::Update(float deltaTime)
{
    m_InputManager->UpdateMousePosition(m_Window);
    
    if (!m_IsGameOver)
    {
        m_Player.Update(deltaTime);
        m_Enemy.Update(deltaTime);
    
        // Move the camera view according to the player's position
        m_CameraView.setCenter(m_Player.GetCenter());
        m_Window.setView(m_CameraView);
    
        // TODO : Clean that
        m_Door.Update(deltaTime);
        m_Ground.Update(deltaTime);
        m_Wall.Update(deltaTime);
        m_Platform.Update(deltaTime);

        //if (m_Door.Contains(m_Player))
        if (m_Door.Contains(m_Player.GetCenter()))
        {
            m_UiManager.StartEndGame();
            m_Door.StartEndGame();
            m_IsGameOver = true;
        }

        // TODO : Clean that
        int bulletNumber = 0;
        auto& bullets = m_Player.GetBullets();
        for (Bullet& b : bullets) {
            bool bulletHadCollision = b.IsColliding(m_Enemy);
            if (b.GetDistance() > 400.f || bulletHadCollision) {
                bullets.erase(bullets.begin() + bulletNumber);
            }

            if (bulletHadCollision) {
                std::cout << "Touched enemy" << std::endl;
                m_Enemy.Damage();
            }

            bulletNumber++;
        }
    }
}

void GameManager::Render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));
    target.draw(m_Player);
    target.draw(m_Enemy);

    target.draw(m_LevelManager);
    
    // TODO : Clean that
    target.draw(m_Door);
    target.draw(m_Wall);
    target.draw(m_Ground);
    target.draw(m_Platform);

    target.draw(m_UiManager);
}

void GameManager::RenderDebugMenu(sf::RenderTarget& target)
{
    ImGui::Begin("Debug Menu");
    ImGui::Text("Press F1 to close this debug menu");
    ImGui::NewLine();

    if (ImGui::CollapsingHeader("Main character position"))
    {
        const auto& mainCharCenterPos = m_Player.GetCenter();

        ImGui::Text("X: %f", mainCharCenterPos.x);
        ImGui::Text("Y: %f", mainCharCenterPos.y);
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