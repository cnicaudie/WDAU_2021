#include <stdafx.h>
#include <Engine/Game.h>
#include <Engine/Input/Bindings/MouseBinding.h>
#include <Engine/Input/Bindings/KeyboardBinding.h>
#include <Engine/Input/Bindings/JoystickButtonBinding.h>
#include <Engine/Input/Bindings/JoystickAxisBinding.h>

static constexpr float APP_MAX_FRAMERATE{ 60.0f };
static const sf::Vector2u APP_INIT_WINDOW_SIZE{ 1024, 768 };

Game::Game(const char* windowTitle)
    : m_Window{ sf::VideoMode(APP_INIT_WINDOW_SIZE.x, APP_INIT_WINDOW_SIZE.y), windowTitle, sf::Style::Titlebar | sf::Style::Close }
    , m_InputManager{ std::make_shared<InputManager>() }
{
    m_Window.setVerticalSyncEnabled(true);
    m_Window.setFramerateLimit(static_cast<uint32_t>(APP_MAX_FRAMERATE));
    m_Window.setActive();
    ImGui::SFML::Init(m_Window);
}

Game::~Game()
{
    ImGui::SFML::Shutdown();
}

void Game::RunGameLoop()
{
    float deltaTime{ 1.0f / APP_MAX_FRAMERATE };
    sf::Clock clock;

    bool toggleImGui = true;

    while (m_Window.isOpen())
    {
        clock.restart();

        sf::Event event;
        while (m_Window.pollEvent(event))
        {
            m_InputManager->ManageInputEvents(event);

            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    m_Window.close();
                    break;
                }
                
                case sf::Event::Resized:
                {
                    break;
                }

                case sf::Event::KeyPressed:
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        m_Window.close();
                    }
                    else if (event.key.code == sf::Keyboard::F1)
                    {
                        toggleImGui = !toggleImGui;
                    }
                    break;
                }

                default:
                    break;
            }

            ImGui::SFML::ProcessEvent(event);
        }

        ImGui::SFML::Update(m_Window, clock.getElapsedTime());

        Update(deltaTime);
        Render(m_Window);
#if _DEBUG
        RenderDebugMenu(m_Window);
#endif

        UpdateGUI(deltaTime);
        RenderGUI(m_Window);

        ImGui::EndFrame();
        if (toggleImGui)
        {
            ImGui::SFML::Render(m_Window);
        }

        m_Window.display();

        deltaTime = clock.getElapsedTime().asSeconds();
    }
}