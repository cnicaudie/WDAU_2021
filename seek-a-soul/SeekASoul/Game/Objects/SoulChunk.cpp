#include <stdafx.h>
#include "SoulChunk.h"
#include <Game/Entities/Player.h>

SoulChunk::SoulChunk(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f pos)
{
	sf::Vector2f textureSize(32.f, 32.f);

	m_Sprite.setTexture(textureManager->GetTextureFromName("SOUL_CHUNK"));
	
	m_Sprite.setOrigin(textureSize * 0.5f);
	m_Sprite.setPosition(pos);
	m_Sprite.setScale(0.25f, 0.25f);

	SetBoundingBox(pos, textureSize * 0.25f);
	SetTrigger(true);

	LOG_INFO("SoulChunk created !");
}

void SoulChunk::Update(float deltaTime) 
{
	// Nothing here for now
	return;
}

void SoulChunk::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{ 
	target.draw(m_Sprite);
}