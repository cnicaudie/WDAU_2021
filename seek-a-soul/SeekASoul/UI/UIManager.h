#pragma once

class UIManager : public sf::Drawable
{
public:
    UIManager(sf::RenderWindow* window);
    ~UIManager();

    void Update(float deltaTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void OnEvent(const Event* evnt);
    void StartEndGame();

    //====================//

    sf::RenderWindow* m_Window;
    sf::View m_GUIView;

    sf::Font m_MainFont;

    sf::Text m_AmmunitionsText;
    sf::Text m_EndgameText;

    // TODO : Make a sound manager
    sf::SoundBuffer m_EndgameSoundBuffer;
    sf::Sound m_EndgameSound;

    bool m_IsPlayingEndGame;
};