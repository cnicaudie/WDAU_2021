#pragma once

namespace SeekASoul
{
    namespace Gameplay
    {
        class Player;
    }

    namespace UI
    {
        class UIViewModel 
        {
        public:
            friend class Gameplay::Player;

            static UIViewModel* GetInstance();
            UIViewModel(const UIViewModel& uiViewModel) = delete;
            void operator=(const UIViewModel& uiViewModel) = delete;

            inline const sf::Vector2f GetPlayerPosition() const { return m_PlayerPosition; };
            inline const int GetHealthPoints() { return m_HealthPoints; };
            inline const int GetMaxHealthPoints() { return m_MaxHealthPoints; };
            inline const unsigned int GetAmmunitionsNumber() const { return m_AmmunitionsNumber; };
            inline const unsigned int GetSoulChunksNumber() const { return m_SoulChuncksCollected; };

        private:
            UIViewModel();
            ~UIViewModel();

            inline void SetPlayerPosition(const sf::Vector2f& position) { m_PlayerPosition = position; };
            inline void SetHealthPoints(int HP) { m_HealthPoints = HP; };
            inline void SetMaxHealthPoints(int maxHP) { m_MaxHealthPoints = maxHP; };
            inline void SetAmmunitionsNumber(unsigned int ammos) { m_AmmunitionsNumber = ammos; };
            inline void SetSoulChunksNumber(unsigned int soulChunks) { m_SoulChuncksCollected = soulChunks; };

            //====================//

            static UIViewModel* m_UIViewModel; // Singleton instance

            // Player infos
            sf::Vector2f m_PlayerPosition;
            int m_HealthPoints;
            int m_MaxHealthPoints;
            unsigned int m_AmmunitionsNumber;
            unsigned int m_SoulChuncksCollected;

            //====================//

            //friend class Game::Player;
        };
    }
}