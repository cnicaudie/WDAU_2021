#pragma once

namespace SeekASoul
{
	namespace Engine
	{
		class Animated
		{
		public:
			Animated(const sf::Vector2i& spriteSize, const sf::Texture& texture);

			void PlayAnimation(int animationState);
			inline void FlipSprite(bool flip)
			{
				if (flip) 
				{
					m_AnimationSprite.scale(-1.f, 1.f);
				}
				else 
				{
					m_AnimationSprite.scale(1.f, 1.f);
				}
			};

			void SetAnimatedSpritePosition(const sf::Vector2f& position);

		protected:
			sf::Sprite m_AnimationSprite;

		private:
			sf::Texture m_TextureSheet;
	
			sf::Vector2i m_SpriteSize;
			sf::IntRect m_SpriteFrame;

			sf::Clock m_Clock;
		};
	}
}