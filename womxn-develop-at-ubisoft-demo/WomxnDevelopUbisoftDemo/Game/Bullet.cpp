#include <stdafx.h>

#include "Bullet.h"

#include <cmath>

Bullet::Bullet(const sf::Texture& texture, const sf::Vector2f& dir, const sf::Vector2f pos)
	: m_Direction(dir), m_Speed(500.f), m_Distance(0.f)
{
	const sf::Vector2f size(static_cast<float>(texture.getSize().x), static_cast<float>(texture.getSize().y));

	m_Sprite.setTexture(texture);
	m_Sprite.setOrigin(size * 0.5f);
	m_Sprite.setPosition(pos);
	m_Sprite.scale(sf::Vector2f(0.5f, 0.5f));
	SetBoundingBox(pos, size);
}

Bullet::~Bullet() {
	std::cout << "Bullet destroyed !" << std::endl;
}

void Bullet::Update(float deltaTime) 
{
	float oldPosX = GetCenter().x;
	sf::Vector2f newPos(m_Direction * m_Speed * deltaTime);
	m_Sprite.move(newPos);
	
	SetCenter(m_Sprite.getPosition());
	m_Distance += std::abs(oldPosX - m_Sprite.getPosition().x);
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Sprite);
}