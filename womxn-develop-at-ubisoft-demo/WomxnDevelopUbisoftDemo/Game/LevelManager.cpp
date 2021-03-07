#include <stdafx.h>
#include "LevelManager.h"

#include <fstream>
#include <string>

LevelManager::LevelManager(const std::shared_ptr<TextureManager>& textureManager) 
	: m_TextureManager{ textureManager }
	, m_Map { textureManager }
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
	std::vector level = GetLevel(levelNumber);
	m_Map.Load(level, sf::Vector2u(m_LevelWidth, m_LevelHeight));
}


const std::vector<int> LevelManager::GetLevel(const int levelNumber)
{
	std::ifstream levelFile(".\\Assets\\level" + std::to_string(levelNumber) + ".txt");
	std::string line;

	std::vector<int> level;

	unsigned int levelWidth = 0;
	unsigned int levelHeight = 0;

	if (levelFile.is_open()) 
	{
		while (std::getline(levelFile, line))
		{
			++levelHeight;

			size_t pos = 0;
			while ((pos = line.find(" ")) != std::string::npos) 
			{
				if (levelHeight == 1) { ++levelWidth; }
				
				int tileNumber = std::stoi(line.substr(0, pos));
				level.push_back(tileNumber);
				line.erase(0, pos + 1);
				
				// Handle while statement skipping last column
				if (line.size() == 1) 
				{ 
					level.push_back(std::stoi(line)); 
				}
			}
		}
	}
	
	m_LevelWidth = levelWidth + 1;
	m_LevelHeight = levelHeight;
	
	return level;
}