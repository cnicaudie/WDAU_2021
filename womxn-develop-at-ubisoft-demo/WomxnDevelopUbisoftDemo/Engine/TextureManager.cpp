#include <stdafx.h>
#include "TextureManager.h"

TextureManager::TextureManager() 
{
	sf::Texture player, enemy, bullet;
	
	player.loadFromFile(".\\Assets\\blue_ball.bmp");
	enemy.loadFromFile(".\\Assets\\blue_ball.bmp");
	bullet.loadFromFile(".\\Assets\\red_ball.bmp");

	m_Textures.emplace("PLAYER", player);
	m_Textures.emplace("ENEMY", enemy);
	m_Textures.emplace("BULLET", bullet);

	std::cout << "Texture manager created" << std::endl;
}