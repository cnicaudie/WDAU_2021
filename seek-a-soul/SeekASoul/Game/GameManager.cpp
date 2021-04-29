#include <stdafx.h>
#include "GameManager.h"
#include <Engine/Input/Bindings/KeyboardBinding.h>
#include <Engine/Input/Bindings/JoystickButtonBinding.h>
#include <Engine/Input/Bindings/JoystickAxisBinding.h>
#include <Engine/Input/Bindings/MouseBinding.h>
#include <Engine/Event/Listener/EventListener.h>
#include <Engine/Collision/CollisionManager.h>
#include <Game/Events/LevelEvent.h>
#include <Game/Camera/CameraManager.h>
#include <Game/Level/LevelManager.h>
#include <UI/UIManager.h>
#include <Game/Actions/Action.h>

namespace SeekASoul
{
    namespace Gameplay
    {
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
            , m_TextureManager{ std::make_shared<Engine::TextureManager>() }
            , m_UIManager{ std::make_unique<UI::UIManager>(&m_Window) }
            , m_LevelManager { std::make_unique<LevelManager>(m_InputManager, m_TextureManager) }
            , m_CameraManager{ std::make_unique<CameraManager>(&m_Window) }
            , m_CurrentState(GameState::NOT_STARTED)
            , m_FramesPerSecond(60)
        {   
            m_CameraManager->SetBoxToFollow(&(m_LevelManager->GetPlayerOnMap()));

            InitBindings();

            // Configure EventListeners
            Engine::EventListener<GameManager, Engine::Event> listenerEvent(this, &GameManager::OnEvent);
            Engine::EventListener<GameManager, LevelEvent> listenerLevelEvent(this, &GameManager::OnEvent);
            Engine::EventManager::GetInstance()->AddListener(listenerEvent);
            Engine::EventManager::GetInstance()->AddListener(listenerLevelEvent);
        }

        void GameManager::InitBindings()
        {
            // TODO : Make a default bindings file to parse at construction

            // === MOVE UP
            
            m_InputManager->AddActionBinding(std::make_shared<Engine::KeyboardBinding>(sf::Keyboard::Up)
                , std::make_shared<Action>(ActionType::MOVE_UP));

            m_InputManager->AddActionBinding(std::make_shared<Engine::KeyboardBinding>(sf::Keyboard::Z)
                , std::make_shared<Action>(ActionType::MOVE_UP));

            m_InputManager->AddActionBinding(std::make_shared<Engine::JoystickButtonBinding>(Engine::JoystickButton::TOP_RIGHT)
                , std::make_shared<Action>(ActionType::MOVE_UP));

            // === MOVE DOWN

            m_InputManager->AddActionBinding(std::make_shared<Engine::KeyboardBinding>(sf::Keyboard::Down)
                , std::make_shared<Action>(ActionType::MOVE_DOWN));

            m_InputManager->AddActionBinding(std::make_shared<Engine::KeyboardBinding>(sf::Keyboard::S)
                , std::make_shared<Action>(ActionType::MOVE_DOWN));

            m_InputManager->AddActionBinding(std::make_shared<Engine::JoystickButtonBinding>(Engine::JoystickButton::TOP_LEFT)
                , std::make_shared<Action>(ActionType::MOVE_DOWN));

            // === MOVE RIGHT

            m_InputManager->AddActionBinding(std::make_shared<Engine::KeyboardBinding>(sf::Keyboard::Right)
                , std::make_shared<Action>(ActionType::MOVE_RIGHT));

            m_InputManager->AddActionBinding(std::make_shared<Engine::KeyboardBinding>(sf::Keyboard::D)
                , std::make_shared<Action>(ActionType::MOVE_RIGHT));

            m_InputManager->AddActionBinding(std::make_shared<Engine::JoystickAxisBinding>(sf::Joystick::Axis::X, true)
                , std::make_shared<Action>(ActionType::MOVE_RIGHT));

            // === MOVE LEFT

            m_InputManager->AddActionBinding(std::make_shared<Engine::KeyboardBinding>(sf::Keyboard::Left)
                , std::make_shared<Action>(ActionType::MOVE_LEFT));

            m_InputManager->AddActionBinding(std::make_shared<Engine::KeyboardBinding>(sf::Keyboard::Q)
                , std::make_shared<Action>(ActionType::MOVE_LEFT));

            m_InputManager->AddActionBinding(std::make_shared<Engine::JoystickAxisBinding>(sf::Joystick::Axis::X, false)
                , std::make_shared<Action>(ActionType::MOVE_LEFT));

            // === SKULL ROLL

            m_InputManager->AddActionBinding(std::make_shared<Engine::KeyboardBinding>(sf::Keyboard::Space)
                , std::make_shared<Action>(ActionType::SKULL_ROLL));
            
            m_InputManager->AddActionBinding(std::make_shared<Engine::JoystickButtonBinding>(Engine::JoystickButton::RIGHT_RIGHT)
                , std::make_shared<Action>(ActionType::SKULL_ROLL));

            // === SHOOT

            m_InputManager->AddActionBinding(std::make_shared<Engine::MouseBinding>(sf::Mouse::Button::Right)
                , std::make_shared<Action>(ActionType::SHOOT));

            m_InputManager->AddActionBinding(std::make_shared<Engine::JoystickAxisBinding>(sf::Joystick::Axis::Z, true)
                , std::make_shared<Action>(ActionType::SHOOT));

            // === AIM X

            m_InputManager->AddActionBinding(std::make_shared<Engine::JoystickAxisBinding>(sf::Joystick::Axis::U, true)
                , std::make_shared<Action>(ActionType::AIM_X));

            m_InputManager->AddActionBinding(std::make_shared<Engine::JoystickAxisBinding>(sf::Joystick::Axis::U, false)
                , std::make_shared<Action>(ActionType::AIM_X));

            // === AIM Y

            m_InputManager->AddActionBinding(std::make_shared<Engine::JoystickAxisBinding>(sf::Joystick::Axis::V, true)
                , std::make_shared<Action>(ActionType::AIM_Y));

            m_InputManager->AddActionBinding(std::make_shared<Engine::JoystickAxisBinding>(sf::Joystick::Axis::V, false)
                , std::make_shared<Action>(ActionType::AIM_Y));
        }

        GameManager::~GameManager()
        {
            Engine::EventListener<GameManager, Engine::Event> listenerEvent(this, &GameManager::OnEvent);
            Engine::EventListener<GameManager, LevelEvent> listenerLevelEvent(this, &GameManager::OnEvent);
            Engine::EventManager::GetInstance()->RemoveListener(listenerEvent);
            Engine::EventManager::GetInstance()->RemoveListener(listenerLevelEvent);

            delete m_GameManager;
        }

        void GameManager::Update(float deltaTime)
        {
            // Update FPS (every second)
            if (m_FPSUpdateClock.getElapsedTime().asSeconds() >= 1.f) 
            {
                m_FramesPerSecond = static_cast<int>(1.f / deltaTime);
                m_FPSUpdateClock.restart();
            }

            // Update Game elements
            Engine::EventManager::GetInstance()->Update();
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
            const float menuWidth = 250.f;
            const float menuHeight = 250.f;

            ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_FirstUseEver);
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
                std::shared_ptr<Engine::Event> eventType = std::make_shared<Engine::Event>(Engine::EventType::END_GAME);
                Engine::EventManager::GetInstance()->Fire(eventType);
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

        const std::pair<bool, bool> GameManager::CheckCollisions(Engine::BoxCollideable* collideable, const sf::Vector2f& positionOffset) const
        {
            return m_CollisionManager->CheckCollisions(collideable, positionOffset, m_LevelManager->GetGameMap().GetGameGrid());
        }

        void GameManager::OnEvent(const Engine::Event* evnt)
        {
            if (evnt->GetEventType() == Engine::EventType::START_GAME)
            {
                m_CurrentState = GameState::PLAYING;
            }
            else if (evnt->GetEventType() == Engine::EventType::END_GAME)
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
    }
}