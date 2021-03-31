#include <stdafx.h>
#include "LevelManager.h"
#include <Engine/Resources/FileReader.h>

LevelManager::LevelManager(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager)
	: m_TextureManager{ textureManager }
	, m_Map { inputManager, textureManager }
{
	LoadLevel(0);
}

void LevelManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Map);
}

void LevelManager::Update(float deltaTime) 
{
	m_Map.Update(deltaTime);
}

void LevelManager::LoadLevel(const int levelNumber = 0)
{
	std::string levelFileName(".\\Assets\\Levels\\level" + std::to_string(levelNumber) + ".txt");
	std::pair<std::vector<int>, sf::Vector2u> levelData = FileReader::ReadLevelFromFile(levelFileName);
	
	m_LevelWidth = levelData.second.x;
	m_LevelHeight = levelData.second.y;

	m_Map.Load(levelData.first, sf::Vector2u(m_LevelWidth, m_LevelHeight));
}