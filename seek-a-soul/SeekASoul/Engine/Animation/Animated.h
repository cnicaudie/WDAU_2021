#pragma once

class Animated
{
public:
	Animated(sf::Vector2i spriteSize);
	void PlayAnimation(int animationState);

private:
	sf::Texture m_TextureSheet;
	sf::IntRect m_SpriteFrame;

	sf::Clock m_Clock;
};