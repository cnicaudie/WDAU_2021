#pragma once

#include "Collectible.h"
#include <Engine/Animation/Animated.h>

namespace SeekASoul
{
	namespace Gameplay
	{
		class SoulChunk : public sf::Drawable, public Collectible, public Engine::Animated
		{
		public:
			SoulChunk(const std::shared_ptr<Engine::TextureManager>& textureManager, const sf::Vector2f pos);

			void Update(float deltaTime);
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		};
	}
}