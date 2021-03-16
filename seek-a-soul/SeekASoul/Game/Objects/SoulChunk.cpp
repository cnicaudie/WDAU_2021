#include <stdafx.h>
#include "SoulChunk.h"
#include <Game/Entities/Player.h>

SoulChunk::SoulChunk(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f pos)
	: m_WasCollected(false)
{
	sf::Vector2f textureSize(32.f, 32.f);

	m_Sprite.setTexture(textureManager->GetTextureFromName("SOUL_CHUNK"));
	// TODO : Don't hardcode the position of the wanted tile 
	//m_Sprite.setTextureRect(sf::IntRect(64, 0, static_cast<int>(textureSize.x), static_cast<int>(textureSize.y)));

	m_Sprite.setOrigin(textureSize * 0.5f);
	m_Sprite.setPosition(pos);
	m_Sprite.setScale(0.25f, 0.25f);

	SetBoundingBox(pos, textureSize * 0.25f);
	SetTrigger(true);
	std::cout << "SoulChunk created !" << std::endl;
}

SoulChunk::~SoulChunk() 
{
	std::cout << "SoulChunk destroyed !" << std::endl;
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

void SoulChunk::OnTrigger(BoxCollideable* other)
{
	if (typeid(*other) == typeid(class Player) && !m_WasCollected)
	{
		std::cout << "Collected soul chunk !" << std::endl;
		m_WasCollected = true;
	}
}