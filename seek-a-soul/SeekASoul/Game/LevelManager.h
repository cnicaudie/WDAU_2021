#pragma once

#include <Game/Map/Map.h>

class LevelManager : public sf::Drawable
{
public:
	LevelManager(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager);

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void RenderDebugMenu(sf::RenderTarget& target) { m_Map.RenderDebugMenu(target); };
	
	void LoadLevel(const int levelNumber);

	inline const Map& GetMap() const { return m_Map; };
	inline const sf::Vector2u GetLevelBounds() { return { m_LevelWidth * Map::TILE_SIZE.x, m_LevelHeight * Map::TILE_SIZE.y }; };
	inline const Player& GetPlayerOnMap() { return m_Map.GetPlayer(); };

private:
	std::shared_ptr<TextureManager> m_TextureManager;
	Map m_Map;

	unsigned int m_LevelWidth;
	unsigned int m_LevelHeight;
};