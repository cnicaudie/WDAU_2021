#pragma once

class UIManager : public sf::Drawable
{
public:
    UIManager();
    ~UIManager();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void StartEndGame();

private:
    // End Game Elements
    sf::Font m_EndgameTextFont;
    sf::Text m_EndgameText;
    sf::SoundBuffer m_EndgameSoundBuffer;
    sf::Sound m_EndgameSound;

    bool m_IsPlayingEndGame;
};