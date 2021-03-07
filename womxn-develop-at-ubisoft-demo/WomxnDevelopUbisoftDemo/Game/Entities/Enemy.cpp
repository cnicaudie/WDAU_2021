#include "stdafx.h"
#include "Enemy.h"
#include <Game/Objects/Bullet.h>

Enemy::Enemy(const std::shared_ptr<TextureManager>& textureManager)
	: Entity(textureManager, { 300.0f, 80.0f }, 50)
	, m_WasDamaged(false)
	, m_DamageCooldown(2.f)
{
	sf::Vector2f textureSize = textureManager->GetTextureSizeFromName("ENEMY");

	m_Sprite.setTexture(textureManager->GetTextureFromName("ENEMY"));
	m_Sprite.setOrigin(textureSize * 0.5f);
	m_Sprite.setPosition(m_Position);
	m_Sprite.setColor(sf::Color::Green);

	SetBoundingBox(m_Position, textureSize);
}

void Enemy::Update(float deltaTime) 
{
	if (m_IsDead) 
	{
		return;
	}
	
	if (m_WasDamaged) 
	{
		if (m_DamageCooldown >= 0.3f) {
			m_WasDamaged = false;
			m_Sprite.setColor(sf::Color::Green);
		}
		else
		{
			m_DamageCooldown += 1.f * deltaTime;
		}
	}
}

void Enemy::OnCollision(const BoxCollideable* other)
{
	if (typeid(*other) == typeid(class Bullet) && !m_WasDamaged)
	{
		Damage();
	}
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	Entity::draw(target, states);
}

void Enemy::Damage() 
{
	std::cout << "Damaged enemy !" << std::endl;
	m_Sprite.setColor(sf::Color::Red);
	m_DamageCooldown = 0.f;
	m_WasDamaged = true;
	m_HealthPoints -= 10;
	
	if (m_HealthPoints == 0) 
	{
		m_IsDead = true;
		std::cout << "Enemy died !" << std::endl;
	}
}