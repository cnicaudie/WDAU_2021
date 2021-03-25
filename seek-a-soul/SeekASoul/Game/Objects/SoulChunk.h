#pragma once

#include "Collectible.h"

class SoulChunk : public sf::Drawable, public Collectible 
{
public:
	SoulChunk(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f pos);
	~SoulChunk();

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
private:
	sf::Sprite m_Sprite;
	sf::Vector2f m_Position;
};