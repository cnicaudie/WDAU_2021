#pragma once

#include <Game/Map/TileMap.h>

class LevelManager : public sf::Drawable
{
public:
	LevelManager(const std::shared_ptr<TextureManager>& textureManager);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void Update(float deltaTime);
	void LoadLevel(const int levelNumber);

	const TileMap& GetMap() const { return m_Map; };

private:
	const std::vector<int> GetLevel(const int levelNumber);

	std::shared_ptr<TextureManager> m_TextureManager;
	TileMap m_Map;

	unsigned int m_LevelWidth;
	unsigned int m_LevelHeight;
};