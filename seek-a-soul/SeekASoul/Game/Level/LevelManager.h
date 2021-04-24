#pragma once

#include <Game/Map/Map.h>

class LevelManager : public sf::Drawable
{
public:
	LevelManager(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager);
	~LevelManager();

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void RenderDebugMenu(sf::RenderTarget& target);
	
	inline const Map& GetMap() const { return m_Map; };
	inline const sf::Vector2u GetLevelBounds() { return { m_LevelWidth * Map::TILE_SIZE.x, m_LevelHeight * Map::TILE_SIZE.y }; };
	inline const Player& GetPlayerOnMap() { return m_Map.GetPlayer(); };

private:
	void ChooseLevel();
	void LoadLevel(bool restart);
	void ManageLevelChange();

	void OnEvent(const Event* evnt);

	//====================//

	std::shared_ptr<TextureManager> m_TextureManager;
	Map m_Map;

	enum class LevelState 
	{
		PENDING		= 0,
		SELECTING	= 1,
		LOADING		= 2,
		PLAYING		= 3,
		OVER		= 4
	} m_CurrentState;

	int m_LevelChoice;
	unsigned int m_CurrentLevel;
	unsigned int m_LevelWidth;
	unsigned int m_LevelHeight;
};