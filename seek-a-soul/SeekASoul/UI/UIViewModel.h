#pragma once

class UIViewModel 
{
public:
    static UIViewModel* GetInstance();
    UIViewModel(const UIViewModel& uiViewModel) = delete;
    void operator=(const UIViewModel& uiViewModel) = delete;

    inline const sf::Vector2f GetPlayerPosition() const { return m_PlayerPosition; };
    inline const int GetAmmunitionsNumber() const { return m_AmmunitionsNumber; };
    inline const int GetSoulChunksNumber() const { return m_SoulChuncksCollected; };

private:
    UIViewModel();
    ~UIViewModel();

    inline void SetPlayerPosition(const sf::Vector2f& position) { m_PlayerPosition = position; };
    inline void SetAmmunitionsNumber(unsigned int number) { m_AmmunitionsNumber = number; };
    inline void SetSoulChunksNumber(unsigned int number) { m_SoulChuncksCollected = number; };

    //====================//

    static UIViewModel* m_UIViewModel; // Singleton instance

    sf::Vector2f m_PlayerPosition;
    unsigned int m_AmmunitionsNumber;
    unsigned int m_SoulChuncksCollected;

    //====================//

    friend class Player;
};