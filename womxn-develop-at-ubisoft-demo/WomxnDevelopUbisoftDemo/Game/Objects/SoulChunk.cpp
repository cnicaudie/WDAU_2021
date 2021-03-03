#include <stdafx.h>
#include "SoulChunk.h"

SoulChunk::SoulChunk(const sf::Texture& texture, const sf::Vector2f pos)
{
	sf::Vector2f textureSize(32.f, 32.f);

	m_Sprite.setTexture(texture);
	// TODO : Don't hardcode the position of the wanted tile 
	m_Sprite.setTextureRect(sf::IntRect(64, 0, static_cast<int>(textureSize.x), static_cast<int>(textureSize.y)));

	m_Sprite.setOrigin(textureSize * 0.5f);
	m_Sprite.setPosition(pos);

	SetBoundingBox(pos, textureSize);
	std::cout << "SoulChunk created !" << std::endl;
}