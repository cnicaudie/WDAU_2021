#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(const std::shared_ptr<TextureManager>& textureManager)
	: m_HealthPoints(100)
	, m_Position(500.0f, 655.5f)
	, m_WasDamaged(false)
	, m_DamageCooldown(2.f)
	, m_IsDead(false)
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

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	if (!m_IsDead)
	{
		target.draw(m_Sprite);
	}

}

void Enemy::Damage() 
{
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