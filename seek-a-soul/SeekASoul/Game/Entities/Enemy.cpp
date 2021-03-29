#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include <Game/Objects/Bullet.h>

static constexpr uint64_t DAMAGE_COOLDOWN = 1000;

Enemy::Enemy(const std::shared_ptr<TextureManager>& textureManager)
	: Entity(textureManager, { 320.0f, 65.0f }, 50)
{
	sf::Vector2f textureSize = textureManager->GetTextureSizeFromName("ENEMY");

	m_Sprite.setTexture(textureManager->GetTextureFromName("ENEMY"));
	m_Sprite.setOrigin(textureSize * 0.5f);
	m_Sprite.setPosition(m_Position);
	m_Sprite.setScale(0.8f, 0.8f);

	SetBoundingBox(m_Position, textureSize * 0.8f);
}

void Enemy::Update(float deltaTime) 
{
	auto time = std::chrono::system_clock::now().time_since_epoch();
	uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
	
	if (m_HealthState == HealthState::DEAD)
	{
		return;
	}
	
	if (m_HealthState == HealthState::DAMAGED)
	{
		UpdateVisualDamage(now);
	}
}

void Enemy::OnCollision(BoxCollideable* other, CollisionDirection direction)
{
	Player* player = dynamic_cast<Player*>(other);

	if (player != nullptr && player->IsSkullRolling() 
		&& (m_HealthState == HealthState::OK))
	{
		Damage();
	}	

	if (typeid(*other) == typeid(class Bullet) 
		&& (m_HealthState == HealthState::OK))
	{
		Damage();
	}
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	if (!(m_HealthState == HealthState::DEAD))
	{
		target.draw(m_Sprite);
	}
}

void Enemy::Damage() 
{
	LOG_INFO("Enemy was damaged !");

	m_Sprite.setColor(sf::Color::Red);
	m_HealthState = HealthState::DAMAGED;
	m_HealthPoints -= 10;

	if (m_HealthPoints == 0)
	{
		m_HealthState = HealthState::DEAD;
		LOG_INFO("Enemy died !");
	}

	auto time = std::chrono::system_clock::now().time_since_epoch();
	m_LastDamageTime = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
}

void Enemy::UpdateVisualDamage(uint64_t now)
{
	if ((now - m_LastDamageTime) >= DAMAGE_COOLDOWN) {
		m_HealthState = HealthState::OK;
		m_Sprite.setColor(sf::Color::White);
	}
}