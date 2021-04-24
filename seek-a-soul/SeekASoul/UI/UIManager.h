#pragma once

#include "Button.h"

class UIManager : public sf::Drawable
{
public:
    UIManager(sf::RenderWindow* window);
    ~UIManager();

    void Update(float deltaTime);
    void ManageButtons();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void InitTexts(const sf::Vector2f& WINDOW_CENTER);
    void InitButtons(const sf::Vector2f& WINDOW_CENTER);

    void OnEvent(const Event* evnt);
    void StartEndGame();

    //====================//

    sf::RenderWindow* m_Window;
    sf::View m_GUIView;

    sf::Font m_MainFont;
    
    // === In Game UI
    sf::Text m_AmmunitionsText;
    sf::Text m_SoulChunksText;
    // TODO : Health bar
    
    // === Main Menu
    bool m_ToggleMainMenu;
    Button m_StartButton;
    Button m_ChooseLevelButton;
    Button m_CloseButton;

    // === End Level/Game Menu
    bool m_IsPlayingEndGame;
    bool m_WentBackToMenu;
    Button m_RestartButton;
    Button m_BackToMenuButton;
    sf::Text m_EndGameText;

    // TODO : Make a sound manager
    sf::SoundBuffer m_EndgameSoundBuffer;
    sf::Sound m_EndgameSound;
};