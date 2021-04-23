#pragma once

class UIManager;
class LevelManager;
class CameraManager;
class CollisionManager;

class GameManager : public Game
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

    const bool CheckCollision(BoxCollideable* collideable, const sf::Vector2f& positionOffset) const;
    const sf::Vector2u GetLevelBounds() const;
    
    inline const bool IsGameOver() const { return m_CurrentState == GameState::OVER; }
    inline const bool HasGameStarted() const { return m_CurrentState != GameState::NOT_STARTED; }
    inline void Restart() { m_CurrentState = GameState::PLAYING; };

private:
    GameManager();
    ~GameManager();

    void OnEvent(const Event* evnt);

    //====================//

    static GameManager* m_GameManager; // Singleton instance

    std::shared_ptr<TextureManager> m_TextureManager;
    std::unique_ptr<UIManager> m_UIManager;
    std::unique_ptr<LevelManager> m_LevelManager;
    std::unique_ptr<CameraManager> m_CameraManager;
    std::unique_ptr<CollisionManager> m_CollisionManager;
    
    enum class GameState
    {
        NOT_STARTED = 0,
        PLAYING     = 1,
        OVER        = 2
    } m_CurrentState;

    sf::Clock m_FPSUpdateClock;
    int m_FramesPerSecond; // For Debug purposes
};