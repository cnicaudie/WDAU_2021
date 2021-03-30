#pragma once

class UIViewModel 
{
public:
    static UIViewModel* GetInstance();
    UIViewModel(const UIViewModel& uiViewModel) = delete;
    void operator=(const UIViewModel& uiViewModel) = delete;

    inline const int GetAmmunitionsNumber() const { return m_AmmunitionsNumber; };
    inline const sf::Vector2f GetPlayerPosition() const { return m_PlayerPosition; };

private:
    UIViewModel();
    ~UIViewModel();

    inline void SetAmmunitionsNumber(int number) { m_AmmunitionsNumber = number; };
    inline void SetPlayerPosition(const sf::Vector2f& position) { m_PlayerPosition = position; };

    //====================//

    static UIViewModel* m_UIViewModel; // Singleton instance

    sf::Vector2f m_PlayerPosition;
    int m_AmmunitionsNumber;

    //====================//

    friend class Player;
};