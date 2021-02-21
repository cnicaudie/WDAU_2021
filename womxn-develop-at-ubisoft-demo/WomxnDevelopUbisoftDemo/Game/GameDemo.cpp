#include "stdafx.h"
#include "GameDemo.h"

#include <vector>

GameDemo::GameDemo()
    : Game{ "Game Demo" }
    , m_MainCharacter{}
    , m_Door{ 900, 600, 100, 200 }
    , m_Ground{ 400, 700, 500, 25 }
    , m_Wall{ 200, 500, 25, 200 }
    , m_Platform{ 400, 500, 100, 25 }
    , m_IsFinished{ false }
    , m_cameraView{}
{
    m_EndgameTextFont.loadFromFile("Assets\\arial.ttf");

    m_EndgameText.setFont(m_EndgameTextFont);
    m_EndgameText.setPosition(500, 400);
    m_EndgameText.setString("!!! WIN !!!");
    m_EndgameText.setCharacterSize(24);
    m_EndgameText.setFillColor(sf::Color::Red);

    m_EndgameSoundBuffer.loadFromFile("Assets\\Test.wav");

    m_EndgameSound.setBuffer(m_EndgameSoundBuffer);

    std::vector<Wall> walls{ m_Wall, m_Ground, m_Platform };
    m_MainCharacter.InitColliders(walls);

    sf::Vector2f viewSize{ 1024, 768 };
    m_cameraView.setSize(viewSize);
    m_cameraView.zoom(1.5f);
    m_cameraView.setCenter(m_MainCharacter.GetCenter());
    //view.setViewport(sf::FloatRect(0.0f, 0.0f, 0.5f, 1.0f));
    m_Window.setView(m_cameraView);
}

void GameDemo::Update(float deltaTime)
{
    if (!m_IsFinished)
    {
        m_MainCharacter.Update(deltaTime);
    
        // Move the camera view according to the player's position
        m_cameraView.setCenter(m_MainCharacter.GetCenter());
        m_Window.setView(m_cameraView);
    
        // TODO : Clean that
        m_Door.Update(deltaTime);
        m_Ground.Update(deltaTime);
        m_Wall.Update(deltaTime);
        m_Platform.Update(deltaTime);

        //if (m_Door.Contains(m_MainCharacter.GetCenter()))
        if (m_Door.Contains(m_MainCharacter))
        {
            m_EndgameSound.play();

            m_MainCharacter.StartEndGame();
            m_Door.StartEndGame();
            m_IsFinished = true;
        }
    }
}

void GameDemo::Render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));
    target.draw(m_MainCharacter);
    
    // TODO : Clean that
    target.draw(m_Door);
    target.draw(m_Wall);
    target.draw(m_Ground);
    target.draw(m_Platform);

    if (m_IsFinished)
    {
        target.draw(m_EndgameText);
    }
}

void GameDemo::RenderDebugMenu(sf::RenderTarget& target)
{
    ImGui::Begin("Debug Menu");
    ImGui::Text("Press F1 to close this debug menu");
    ImGui::NewLine();

    if (ImGui::CollapsingHeader("Main character position"))
    {
        const auto& mainCharCenterPos = m_MainCharacter.GetCenter();

        ImGui::Text("X: %f", mainCharCenterPos.x);
        ImGui::Text("Y: %f", mainCharCenterPos.y);
    }

    if (ImGui::CollapsingHeader("Game status"))
    {
        if (m_IsFinished)
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