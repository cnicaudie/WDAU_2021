#pragma once

#include <Engine/Input/InputManager.h>

class Game
{
public:
    Game(const char* windowTitle);
    virtual ~Game();

    void RunGameLoop();

protected:
    virtual void Update(float deltaTime) = 0;
    virtual void Render(sf::RenderTarget& target) = 0;
    virtual void RenderDebugMenu(sf::RenderTarget& target) = 0;

    sf::RenderWindow m_Window;
    std::shared_ptr<InputManager> m_InputManager;
};