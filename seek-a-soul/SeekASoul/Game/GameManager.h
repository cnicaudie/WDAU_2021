#pragma once

namespace SeekASoul
{
    namespace UI 
    {
        class UIManager;
    }

    namespace Engine 
    {
        class CollisionManager;
    }

    namespace Gameplay
    {
        class LevelManager;
        class CameraManager;

        class GameManager : public Engine::Game
        {
        public:
            static GameManager* GetInstance();
            GameManager(const GameManager& gameManager) = delete;
            void operator=(const GameManager& gameManager) = delete;

            void Update(float deltaTime) override;
            void UpdateGUI(float deltaTime) override;
            void Render(sf::RenderTarget& target) override;
            void RenderGUI(sf::RenderTarget& target) override;
            void RenderDebugMenu(sf::RenderTarget& target) override;

            const std::pair<bool, bool> CheckCollisions(Engine::BoxCollideable* collideable, const sf::Vector2f& positionOffset) const;
            const sf::Vector2u GetLevelBounds() const;
    
            inline const bool IsGameOver() const { return m_CurrentState == GameState::OVER; }
            inline const bool HasGameStarted() const { return m_CurrentState != GameState::NOT_STARTED; }
            inline void ResetGameState() { m_CurrentState = GameState::NOT_STARTED; };

        private:
            GameManager();
            ~GameManager();

            void OnEvent(const Engine::Event* evnt);

            //====================//

            static GameManager* m_GameManager; // Singleton instance

            std::shared_ptr<Engine::TextureManager> m_TextureManager;
            std::unique_ptr<Engine::CollisionManager> m_CollisionManager;
            std::unique_ptr<UI::UIManager> m_UIManager;
            std::unique_ptr<LevelManager> m_LevelManager;
            std::unique_ptr<CameraManager> m_CameraManager;
    
            enum class GameState
            {
                NOT_STARTED = 0,
                PLAYING     = 1,
                OVER        = 2
            } m_CurrentState;

            sf::Clock m_FPSUpdateClock;
            int m_FramesPerSecond; // For Debug purposes
        };
    }
}