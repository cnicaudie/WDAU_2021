#include "stdafx.h"
#include "Enemy.h"

#include <iostream>

Enemy::Enemy()
	: m_HealthPoints(100), m_Position(500.0f, 655.5f), m_wasDamaged(false), m_damageCooldown(2.f)
{
	m_Texture.loadFromFile(".\\Assets\\blue_ball.bmp");
	m_Texture.setSmooth(true);
	
	const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));

	m_Sprite.setTexture(m_Texture);
	m_Sprite.setOrigin(size * 0.5f);
	m_Sprite.setPosition(m_Position);

	m_Sprite.setColor(sf::Color::Green);

	SetBoundingBox(m_Position, size);

}

void Enemy::Update(float deltaTime) 
{
	// TODO
	if (m_wasDamaged) 
	{
		if (m_damageCooldown >= 0.3f) {
			m_wasDamaged = false;
			m_Sprite.setColor(sf::Color::Green);
		}
		else
		{
			m_damageCooldown += 1.f * deltaTime;
		}
	}
	
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	target.draw(m_Sprite);
}

void Enemy::Damage() 
{
	m_Sprite.setColor(sf::Color::Red);
	m_HealthPoints -= 10;
	m_damageCooldown = 0.f;
	m_wasDamaged = true;
}