#include <stdafx.h>
#include "UIManager.h"
#include <Game/GameManager.h>
#include <Game/Level/LevelStatus.h>
#include <Game/Events/LevelEvent.h>
#include <UI/UIViewModel.h>
#include <sstream>

namespace SeekASoul
{
    namespace UI
    {
        static const sf::Vector2f BUTTON_SIZE{ 180.f, 50.f };

        UIManager::UIManager(sf::RenderWindow* window)
            : m_Window(window)
            , m_GUIView(sf::FloatRect(0.f, 0.f, static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)))
            , m_ToggleMainMenu(true)
            , m_IsPlayingEndGame(false)
            , m_WentBackToMenu(false)
            , m_StartButton(BUTTON_SIZE)
            , m_ChooseLevelButton(BUTTON_SIZE)
            , m_CloseButton(BUTTON_SIZE)
            , m_RestartButton(BUTTON_SIZE)
            , m_BackToMenuButton(BUTTON_SIZE)
        {
            const sf::Vector2f WINDOW_CENTER{ m_GUIView.getCenter() };
    
            // TODO : Move in a sound manager
            m_EndgameSoundBuffer.loadFromFile("Assets\\Sounds\\Test.wav");
            m_EndgameSound.setBuffer(m_EndgameSoundBuffer);

            m_MainFont.loadFromFile("Assets\\Fonts\\arial.ttf");
    
            InitTexts(WINDOW_CENTER);
            InitButtons(WINDOW_CENTER);

            // Configure EventListeners
            Engine::EventListener<UIManager, Engine::Event> listenerGameOver(this, &UIManager::OnEvent);
            Engine::EventListener<UIManager, Gameplay::LevelEvent> listenerLevelEnd(this, &UIManager::OnEvent);
            Engine::EventManager::GetInstance()->AddListener(listenerGameOver);
            Engine::EventManager::GetInstance()->AddListener(listenerLevelEnd);
        }

        void UIManager::InitTexts(const sf::Vector2f& WINDOW_CENTER)
        {
            // === In Game UI
            m_AmmunitionsText.setFont(m_MainFont);
            m_AmmunitionsText.setCharacterSize(25);
            m_AmmunitionsText.setFillColor(sf::Color::White);
            m_AmmunitionsText.setPosition(WINDOW_CENTER.x * 1.5f, WINDOW_CENTER.y * 0.05f);

            m_SoulChunksText.setFont(m_MainFont);
            m_SoulChunksText.setCharacterSize(25);
            m_SoulChunksText.setFillColor(sf::Color::Cyan);
            m_SoulChunksText.setPosition(WINDOW_CENTER.x * 1.5f, WINDOW_CENTER.y * 0.15f);

            // === Main Menu
            m_MainTitle.setFont(m_MainFont);
            m_MainTitle.setCharacterSize(80);
            m_MainTitle.setFillColor(sf::Color::White);
            m_MainTitle.setString("Seek A Soul");
            m_MainTitle.setStyle(sf::Text::Bold);
            m_MainTitle.setPosition(WINDOW_CENTER.x - (m_MainTitle.getGlobalBounds().width / 2), WINDOW_CENTER.y * 0.15f);

            // === End Level/Game Menu

            m_EndGameText.setFont(m_MainFont);
            m_EndGameText.setCharacterSize(50);
            m_EndGameText.setFillColor(sf::Color::Green);
            m_EndGameText.setString("YOU WON !!!");
            m_EndGameText.setStyle(sf::Text::Bold);
            m_EndGameText.setPosition(WINDOW_CENTER.x - (m_EndGameText.getGlobalBounds().width / 2)
                , WINDOW_CENTER.y - (m_EndGameText.getGlobalBounds().height / 2));
        }

        void UIManager::InitButtons(const sf::Vector2f& WINDOW_CENTER)
        {
            const float BUTTONS_OFFSET = 150.f;

            // === Main Menu

            const sf::Vector2f startButtonPosition{ WINDOW_CENTER.x, WINDOW_CENTER.y - BUTTONS_OFFSET };
            m_StartButton.SetButtonPosition(startButtonPosition);
            m_StartButton.SetButtonTextFont(m_MainFont);
            m_StartButton.SetButtonTextString("Start");
            m_StartButton.SetButtonTextPosition(startButtonPosition);
    
            m_ChooseLevelButton.SetButtonPosition(WINDOW_CENTER);
            m_ChooseLevelButton.SetButtonTextFont(m_MainFont);
            m_ChooseLevelButton.SetButtonTextString("Choose Level");
            m_ChooseLevelButton.SetButtonTextPosition(WINDOW_CENTER);
    
            const sf::Vector2f closeButtonPosition{ WINDOW_CENTER.x, WINDOW_CENTER.y + BUTTONS_OFFSET };
            m_CloseButton.SetButtonPosition(closeButtonPosition);
            m_CloseButton.SetButtonTextFont(m_MainFont);
            m_CloseButton.SetButtonTextString("Quit");
            m_CloseButton.SetButtonTextPosition(closeButtonPosition);

            // === End Level/Game Menu

            const sf::Vector2f restartButtonPosition{ WINDOW_CENTER.x - 2 * BUTTONS_OFFSET, WINDOW_CENTER.y + BUTTONS_OFFSET };
            m_RestartButton.SetButtonPosition(restartButtonPosition);
            m_RestartButton.SetButtonTextFont(m_MainFont);
            m_RestartButton.SetButtonTextString("Restart");
            m_RestartButton.SetButtonTextPosition(restartButtonPosition);

            const sf::Vector2f backButtonPosition{ WINDOW_CENTER.x + 2 * BUTTONS_OFFSET, WINDOW_CENTER.y + BUTTONS_OFFSET };
            m_BackToMenuButton.SetButtonPosition(backButtonPosition);
            m_BackToMenuButton.SetButtonTextFont(m_MainFont);
            m_BackToMenuButton.SetButtonTextString("Back to menu");
            m_BackToMenuButton.SetButtonTextPosition(backButtonPosition);
        }

        UIManager::~UIManager() 
        {
            // Remove listeners
            Engine::EventListener<UIManager, Engine::Event> listenerGameOver(this, &UIManager::OnEvent);
            Engine::EventListener<UIManager, Gameplay::LevelEvent> listenerLevelEnd(this, &UIManager::OnEvent);
            Engine::EventManager::GetInstance()->RemoveListener(listenerGameOver);
            Engine::EventManager::GetInstance()->RemoveListener(listenerLevelEnd);
        }

        void UIManager::Update(float deltaTime)
        {
            // Update texts
            std::stringstream stream;
            stream << "Bones ammos : " << UIViewModel::GetInstance()->GetAmmunitionsNumber();
            m_AmmunitionsText.setString(stream.str());
    
            // Clear the stream content
            stream.str(std::string()); // resets the string content
            stream.clear(); // clears fails and eof flags

            stream << "Soul chunks : " << UIViewModel::GetInstance()->GetSoulChunksNumber();
            m_SoulChunksText.setString(stream.str());

            // Update the view
            m_Window->setView(m_GUIView);

            // Manage buttons
            ManageButtons();
        }

        void UIManager::ManageButtons()
        {
            if (m_ToggleMainMenu)
            {
                if (m_StartButton.WasClicked())
                {
                    LOG_INFO("Starting game...");
                    m_StartButton.ResetClickStatus();
                    m_ToggleMainMenu = false;
            
                    if (m_WentBackToMenu)
                    {
                        std::shared_ptr<Gameplay::LevelEvent> levelEvent = std::make_shared<Gameplay::LevelEvent>(Gameplay::LevelStatus::RESTART);
                        Engine::EventManager::GetInstance()->Fire(levelEvent);
                        m_WentBackToMenu = false;
                    }
                    else
                    {
                        std::shared_ptr<Engine::Event> evnt = std::make_shared<Engine::Event>(Engine::EventType::START_GAME);
                        Engine::EventManager::GetInstance()->Fire(evnt);
                    }
                }
                else if (m_ChooseLevelButton.WasClicked())
                {
                    m_ChooseLevelButton.ResetClickStatus();

                    std::shared_ptr<Gameplay::LevelEvent> levelEvent = std::make_shared<Gameplay::LevelEvent>(Gameplay::LevelStatus::SELECT);
                    Engine::EventManager::GetInstance()->Fire(levelEvent);
                }
                else if (m_CloseButton.WasClicked())
                {
                    LOG_INFO("Bye !");
                    m_Window->close();
                }
            }
            else if (m_IsPlayingEndGame)
            {
                if (m_RestartButton.WasClicked())
                {
                    LOG_INFO("Restarting level...");
                    m_RestartButton.ResetClickStatus();
                    m_IsPlayingEndGame = false;

                    std::shared_ptr<Gameplay::LevelEvent> levelEvent = std::make_shared<Gameplay::LevelEvent>(Gameplay::LevelStatus::RESTART);
                    Engine::EventManager::GetInstance()->Fire(levelEvent);
                }
                else if (m_BackToMenuButton.WasClicked())
                {
                    LOG_INFO("Going back to the main menu...");
                    m_ToggleMainMenu = true;
                    m_WentBackToMenu = true;
                    m_IsPlayingEndGame = false;
                    Gameplay::GameManager::GetInstance()->ResetGameState();
                }
            }
        }

        void UIManager::draw(sf::RenderTarget& target, sf::RenderStates states) const 
        {
            if (m_ToggleMainMenu) 
            {
                target.draw(m_MainTitle);
                target.draw(m_StartButton);
                target.draw(m_ChooseLevelButton);
                target.draw(m_CloseButton);
            }
            else 
            {
                target.draw(m_AmmunitionsText);
                target.draw(m_SoulChunksText);
                ShowHealthBar();

                if (m_IsPlayingEndGame) 
                {
                    target.draw(m_RestartButton);
                    target.draw(m_BackToMenuButton);
                    target.draw(m_EndGameText);
                }
            }   
        }

        void UIManager::OnEvent(const Engine::Event* evnt)
        {
            if (evnt->GetEventType() == Engine::EventType::END_GAME)
            {
                StartEndGame();
            }
            else if (const Gameplay::LevelEvent* actionEvent = dynamic_cast<const Gameplay::LevelEvent*>(evnt))
            {
                switch (actionEvent->GetLevelStatus()) 
                {
                    case Gameplay::LevelStatus::SUCCESS:
                    {
                        m_EndGameText.setFillColor(sf::Color::Green);
                        m_EndGameText.setString("YOU WON !!!");
                        break;
                    }

                    case Gameplay::LevelStatus::FAILURE:
                    {
                        m_EndGameText.setFillColor(sf::Color::Red);
                        m_EndGameText.setString("YOU LOST...");
                        break;
                    }

                    case Gameplay::LevelStatus::RESTART:
                    {
                        m_IsPlayingEndGame = false;
                        m_ToggleMainMenu = false;
                        break;
                    }
                }
            }
        }

        void UIManager::ShowHealthBar() const
        {
            const float menuWidth = 270.f;
            const float menuHeight = 60.f;

            const int HP = UIViewModel::GetInstance()->GetHealthPoints();
            const int maxHP = UIViewModel::GetInstance()->GetMaxHealthPoints();
            const float healthProgress = static_cast<float>(HP) / static_cast<float>(maxHP);

            ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowPos(ImVec2(m_GUIView.getCenter().x - (menuWidth / 2.f), m_GUIView.getCenter().y * 0.05f), ImGuiCond_FirstUseEver);
            ImGui::Begin("Player's HP");
            ImGui::ProgressBar(healthProgress, ImVec2(static_cast<float>(-FLT_MIN), 0.0f), std::to_string(HP).c_str());
            ImGui::End();
        }

        void UIManager::StartEndGame() 
        {
            m_IsPlayingEndGame = true;
            m_EndgameSound.play();
        }
    }
}
