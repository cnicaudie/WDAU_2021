#pragma once

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

	inline const sf::FloatRect GetAnimatedSpriteBoundingBox() const { return m_AnimationSprite.getGlobalBounds(); };
	inline const sf::Vector2i GetSpriteSize() const { return m_SpriteSize; };

protected:
	sf::Sprite m_AnimationSprite;

private:
	sf::Texture m_TextureSheet;
	
	sf::Vector2i m_SpriteSize;
	sf::IntRect m_SpriteFrame;

	sf::Clock m_Clock;
};