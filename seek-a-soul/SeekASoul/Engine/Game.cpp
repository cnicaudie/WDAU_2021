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

                // ==== Inputs management 

                case sf::Event::KeyPressed:
                {
                    m_InputManager->AddAction(std::make_shared<KeyboardBinding>(event.key.code));

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

                case sf::Event::KeyReleased:
                {
                    m_InputManager->RemoveAction(std::make_shared<KeyboardBinding>(event.key.code));
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    m_InputManager->AddAction(std::make_shared<MouseBinding>(event.mouseButton.button));
                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    m_InputManager->RemoveAction(std::make_shared<MouseBinding>(event.mouseButton.button));
                    break;
                }

                case sf::Event::JoystickButtonPressed:
                {
                    m_InputManager->AddAction(std::make_shared<JoystickButtonBinding>(event.joystickButton.button));
                    break;
                }

                case sf::Event::JoystickButtonReleased:
                {
                    m_InputManager->RemoveAction(std::make_shared<JoystickButtonBinding>(event.joystickButton.button));
                    break;
                }

                case sf::Event::JoystickMoved:
                {
                    // Min threshold to consider a "press" on the the axis
                    const float PRESSED_THRESHOLD = 60.f;

                    float joystickPosition = event.joystickMove.position;
                    sf::Joystick::Axis joystickAxis = event.joystickMove.axis;

                    if (joystickAxis == sf::Joystick::Axis::Z) // Only axis used for actions for now
                    {
                        if (joystickPosition >= PRESSED_THRESHOLD || joystickPosition <= -PRESSED_THRESHOLD)
                        {
                            m_InputManager->AddAction(std::make_shared<JoystickAxisBinding>(joystickAxis, joystickPosition > 0));
                        } 
                        else 
                        {
                            m_InputManager->RemoveAction(std::make_shared<JoystickAxisBinding>(joystickAxis, joystickPosition > 0));
                        }
                    }
                    break;
                }

                default:
                    break;
            }

            ImGui::SFML::ProcessEvent(event);
        }

        ImGui::SFML::Update(m_Window, clock.restart());

        Update(deltaTime);
        Render(m_Window);
        RenderDebugMenu(m_Window);

        ImGui::EndFrame();
        if (toggleImGui)
        {
            ImGui::SFML::Render(m_Window);
        }

        m_Window.display();

        deltaTime = clock.getElapsedTime().asSeconds();
    }
}