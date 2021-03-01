#pragma once

#include "TileMap.h"

class LevelManager : public sf::Drawable
{
public:
	LevelManager(const std::shared_ptr<TextureManager>& textureManager);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void LoadLevel(const int levelNumber);

private:
	const std::vector<int> GetLevel(const int levelNumber);

	std::shared_ptr<TextureManager> m_TextureManager;
	TileMap m_Map;

	unsigned int m_LevelWidth;
	unsigned int m_LevelHeight;
};