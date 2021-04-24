#pragma once

#include "Collectible.h"
#include <Engine/Animation/Animated.h>

class SoulChunk : public sf::Drawable, public Collectible, public Animated
{
public:
	SoulChunk(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f pos);

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};