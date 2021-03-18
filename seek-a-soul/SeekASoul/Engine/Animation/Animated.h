#pragma once

class Animated
{
public:
	Animated(const sf::Vector2i& spriteSize, const sf::Texture& texture);

	void PlayAnimation(int animationState);

	void SetAnimatedSpritePosition(const sf::Vector2f& position);

	inline const sf::FloatRect GetSpriteBoundingBox() const { return m_AnimationSprite.getGlobalBounds(); };
	inline const sf::Vector2i GetSpriteSize() const { return m_SpriteSize; };

protected:
	sf::Sprite m_AnimationSprite;

private:
	sf::Texture m_TextureSheet;
	
	sf::Vector2i m_SpriteSize;
	sf::IntRect m_SpriteFrame;

	sf::Clock m_Clock;
};