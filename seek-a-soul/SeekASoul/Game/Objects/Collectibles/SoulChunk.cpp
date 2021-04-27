#include <stdafx.h>
#include "SoulChunk.h"
#include <Game/Entities/Player.h>

namespace SeekASoul
{
	namespace Gameplay
	{
		static const sf::Vector2i SOULCHUNK_SPRITE_SIZE{ 32, 32 };

		SoulChunk::SoulChunk(const std::shared_ptr<Engine::TextureManager>& textureManager, const sf::Vector2f pos)
			: Animated(SOULCHUNK_SPRITE_SIZE, textureManager->GetTextureFromName("SOULCHUNK_SHEET"))
		{
			SetAnimatedSpritePosition(pos);
			SetBoundingBox(pos, static_cast<sf::Vector2f>(SOULCHUNK_SPRITE_SIZE));
			SetTrigger(true);

			LOG_INFO("SoulChunk created !");
		}

		void SoulChunk::Update(float deltaTime) 
		{
			PlayAnimation(0);
		}

		void SoulChunk::draw(sf::RenderTarget& target, sf::RenderStates states) const 
		{ 
			target.draw(m_AnimationSprite);
		}
	}
}