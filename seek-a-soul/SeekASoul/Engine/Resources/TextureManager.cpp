#include <stdafx.h>
#include "TextureManager.h"

TextureManager::TextureManager() 
{
	sf::Texture enemy, bullet, soulChunk;
	
	enemy.loadFromFile(".\\Assets\\Sprites\\Enemy.png");
	bullet.loadFromFile(".\\Assets\\Sprites\\red_ball.bmp");
	soulChunk.loadFromFile(".\\Assets\\Sprites\\blue_ball.bmp");

	m_Textures.emplace("ENEMY", enemy);
	m_Textures.emplace("BULLET", bullet);
	m_Textures.emplace("SOUL_CHUNK", soulChunk);

	sf::Texture player_sheet;
	player_sheet.loadFromFile(".\\Assets\\Sprites\\SkeletonSheet.png");
	m_Textures.emplace("PLAYER_SHEET", player_sheet);

	sf::Texture tileset;
	tileset.loadFromFile(".\\Assets\\Sprites\\Tileset_Basic.png");
	m_Textures.emplace("TILESET", tileset);

	std::cout << "Texture manager created !" << std::endl;
}