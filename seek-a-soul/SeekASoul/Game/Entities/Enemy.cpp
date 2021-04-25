#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include <Engine/Time/Time.h>
#include <Engine/Maths/Maths.h>
#include <Game/Objects/Bullet.h>
#include <Game/Map/Tiles/CollideableTile.h>

static constexpr uint64_t DAMAGE_COOLDOWN = 1000;
static constexpr float MOVE_SPEED = 100.0f;
static constexpr float GRAVITY = 9.8f;

Enemy::Enemy(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& position)
	: Entity(textureManager, position, 50)
{
	sf::Vector2f textureSize = textureManager->GetTextureSizeFromName("ENEMY");

	m_Sprite.setTexture(textureManager->GetTextureFromName("ENEMY"));
	m_Sprite.setOrigin(textureSize * 0.5f);
	m_Sprite.setPosition(m_Position);
	m_Sprite.setScale(0.8f, 0.8f);

	SetBoundingBox(m_Position, textureSize * 0.8f);

	m_Velocity.x = MOVE_SPEED;
}

Enemy::~Enemy() 
{
	LOG_INFO("Destroyed enemy!");
}

void Enemy::Update(float deltaTime) 
{
	uint64_t now = Time::GetCurrentTimeAsMilliseconds();
	
	if (m_HealthState == HealthState::DEAD)
	{
		return;
	}
	
	if (m_HealthState == HealthState::DAMAGED)
	{
		UpdateVisualDamage(now);
	}

	Move(deltaTime);
}

void Enemy::OnCollision(BoxCollideable* other, CollisionDirection direction)
{
	sf::FloatRect otherCollider = other->GetBoundingBox();
	int32_t collisionDirection = static_cast<int32_t>(direction);

	Player* player = dynamic_cast<Player*>(other);

	if ((m_HealthState == HealthState::OK)
		&& ((player != nullptr && player->IsAttacking()) 
			|| (typeid(*other) == typeid(class Bullet))))
	{
		Damage();
	}	

	if (typeid(*other) == typeid(class CollideableTile))
	{
		if (collisionDirection & static_cast<int32_t>(CollisionDirection::BOTTOM))
		{
			m_IsGrounded = true;
			m_Velocity.y = 0.f;
			m_Position.y = otherCollider.top - (m_BoundingBox.height / 2);
		}
		else if (collisionDirection & static_cast<int32_t>(CollisionDirection::TOP))
		{
			m_Velocity.y = GRAVITY;
			m_Position.y = otherCollider.top + otherCollider.height + (m_BoundingBox.height / 2);
		}
		else if (collisionDirection & static_cast<int32_t>(CollisionDirection::LEFT))
		{
			m_Velocity.x = MOVE_SPEED;
			m_Position.x = otherCollider.left + otherCollider.width + (m_BoundingBox.width / 2);
		}
		else if (collisionDirection & static_cast<int32_t>(CollisionDirection::RIGHT))
		{
			m_Velocity.x = -MOVE_SPEED;
			m_Position.x = otherCollider.left - (m_BoundingBox.width / 2);
		}
	}
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	if (!(m_HealthState == HealthState::DEAD))
	{
		target.draw(m_Sprite);
	}
}

void Enemy::Move(float deltaTime) 
{
	sf::Vector2f tempVelocity(0.f, 0.f);

	// Apply gravity
	m_Velocity.y += GRAVITY; // put the gravity const in Entity
	
	// Check movement on X axis
	tempVelocity.x = m_Velocity.x;
	if (!GameManager::GetInstance()->CheckCollision(this, tempVelocity * deltaTime))
	{
		m_Position += tempVelocity * deltaTime;
	}

	// Check movement on Y axis
	tempVelocity.x = 0.0f;
	tempVelocity.y = m_Velocity.y;
	if (!GameManager::GetInstance()->CheckCollision(this, tempVelocity * deltaTime))
	{
		if (m_IsGrounded) 
		{
			m_Velocity.x = -m_Velocity.x;
			m_Velocity.y = -10.f; // little force to help the enemy stay on its platform
			tempVelocity = m_Velocity;
			m_IsGrounded = false;
		}
		m_Position += tempVelocity * deltaTime;
	}

	// Apply new position
	SetCenter(m_Position);
	m_Sprite.setPosition(m_Position);
}

void Enemy::Damage() 
{
	LOG_INFO("Enemy was damaged !");

	m_Sprite.setColor(sf::Color::Red);
	m_HealthState = HealthState::DAMAGED;
	m_HealthPoints -= static_cast<unsigned int>(Maths::GetRandom(5.f, 15.f));
	
	if (m_HealthPoints <= 0)
	{
		m_HealthState = HealthState::DEAD;
		LOG_INFO("Enemy died !");
	}

	m_LastDamageTime = Time::GetCurrentTimeAsMilliseconds();
}

void Enemy::UpdateVisualDamage(uint64_t now)
{
	if (Maths::GetDifference(now, m_LastDamageTime) >= DAMAGE_COOLDOWN)
	{
		m_HealthState = HealthState::OK;
		m_Sprite.setColor(sf::Color::White);
	}
}