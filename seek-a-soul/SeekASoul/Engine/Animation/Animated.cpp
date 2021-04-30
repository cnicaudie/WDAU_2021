#include <stdafx.h>
#include "Animated.h"

namespace SeekASoul
{
	namespace Engine
	{
		Animated::Animated(const sf::Vector2i& spriteSize, const sf::Texture& texture) 
			: m_TextureSheet(texture)
			, m_SpriteSize(spriteSize)
			, m_SpriteFrame{ 0, 0, spriteSize.x, spriteSize.y }
			, m_AnimationSprite(texture, m_SpriteFrame)
		{
			m_AnimationSprite.setOrigin(0.5f * static_cast<sf::Vector2f>(spriteSize));
			m_Clock.restart();
		}

		void Animated::PlayAnimation(const int animationState, const float animationRate)
		{
			if (m_Clock.getElapsedTime().asSeconds() >= animationRate)
			{
				m_SpriteFrame.top = animationState * m_SpriteSize.y;
				m_SpriteFrame.left += m_SpriteSize.x;

				if (m_SpriteFrame.left >= static_cast<int>(m_TextureSheet.getSize().x))
				{
					m_SpriteFrame.left = 0;
				}

				m_AnimationSprite.setTextureRect(m_SpriteFrame);

				m_Clock.restart();
			}
		}

		void Animated::SetAnimatedSpritePosition(const sf::Vector2f& position)
		{
			m_AnimationSprite.setPosition(position);
		}
	}
}