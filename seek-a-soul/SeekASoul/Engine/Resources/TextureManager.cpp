#include <stdafx.h>
#include "TextureManager.h"

namespace SeekASoul
{
	namespace Engine
	{
		TextureManager::TextureManager() 
		{
			sf::Texture enemy, bullet;
	
			enemy.loadFromFile(".\\Assets\\Sprites\\Enemy.png");
			bullet.loadFromFile(".\\Assets\\Sprites\\red_ball.bmp");
	
			m_Textures.emplace("ENEMY", enemy);
			m_Textures.emplace("BULLET", bullet);
	
			sf::Texture player_sheet;
			player_sheet.loadFromFile(".\\Assets\\Sprites\\SkeletonSheet.png");
			m_Textures.emplace("PLAYER_SHEET", player_sheet);

			sf::Texture soulChunk_sheet;
			soulChunk_sheet.loadFromFile(".\\Assets\\Sprites\\SoulChunkSheet.png");
			m_Textures.emplace("SOULCHUNK_SHEET", soulChunk_sheet);

			sf::Texture tileset;
			tileset.loadFromFile(".\\Assets\\Sprites\\Tileset_Basic.png");
			m_Textures.emplace("TILESET", tileset);

			LOG_INFO("Texture manager created !");
		}
	}
}