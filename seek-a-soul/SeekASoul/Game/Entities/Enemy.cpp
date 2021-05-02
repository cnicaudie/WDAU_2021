#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include <Engine/Time/Time.h>
#include <Engine/Maths/Maths.h>
#include <Game/GameplayIncludes.h>
#include <Game/Objects/Bone.h>
#include <Game/Map/Tiles/CollideableTile.h>
#include <Game/Map/Tiles/DeadlyTile.h>
#include <AI/Threat/ThreatTeam.h>
#include <AI/Threat/ThreatManager.h>
#include <Common/ActionHelper.h>

namespace SeekASoul
{
	namespace Gameplay
	{
		static const sf::Vector2i ENEMY_SPRITE_SIZE{ 58, 50 };

		static constexpr uint64_t DAMAGE_COOLDOWN = 800;
		static constexpr float MOVE_SPEED = 100.0f;
		static constexpr float STAY_ON_PLATFORM_FORCE = 50.0f;
		static constexpr float GRAVITY = 9.8f;

		Enemy::Enemy(const std::shared_ptr<Engine::TextureManager>& textureManager, const sf::Vector2f& position)
			: Entity(textureManager, position, 50)
			, Engine::Animated(ENEMY_SPRITE_SIZE, textureManager->GetTextureFromName("ENEMY_SHEET"))
			, AI::AIEntity(AI::ThreatTeam::B)
		{
			SetBoundingBox(m_Position, static_cast<sf::Vector2f>(ENEMY_SPRITE_SIZE));
			
			// Register threat
			AI::ThreatManager::GetInstance()->RegisterThreat(this, &m_Position);

			// Set up possible actions
			m_CanMoveLeft = true;
			m_CanMoveRight = true;
		}

		Enemy::~Enemy() 
		{
			AI::ThreatManager::GetInstance()->UnregisterThreat(this);
			LOG_INFO("Destroyed enemy!");
		}

		void Enemy::Update(float deltaTime) 
		{
			if (m_HealthState == HealthState::DEAD || GameManager::GetInstance()->IsGameOver())
			{
				return;
			}
	
			uint64_t now = Engine::Time::GetCurrentTimeAsMilliseconds();
	
			if (m_HealthState == HealthState::DAMAGED)
			{
				UpdateVisualDamage(now);
			}

			Move(deltaTime);
			PlayAnimation(0);
		}

		void Enemy::OnCollision(Engine::BoxCollideable* other, Engine::CollisionDirection direction)
		{
			sf::FloatRect otherCollider = other->GetBoundingBox();
			int32_t collisionDirection = static_cast<int32_t>(direction);

			Player* player = dynamic_cast<Player*>(other);

			if ((m_HealthState == HealthState::OK)
				&& ((player != nullptr && player->IsAttacking()) 
					|| (typeid(*other) == typeid(class Bone))))
			{
				Damage();
			}	

			if (typeid(*other) == typeid(class CollideableTile) || typeid(*other) == typeid(class DeadlyTile))
			{	
				if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::BOTTOM))
				{
					m_IsGrounded = true;
					m_Velocity.y = 0.f;
					m_Position.y = otherCollider.top - (m_BoundingBox.height / 2);
				}
				else if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::TOP))
				{
					m_Velocity.y = GRAVITY;
					m_Position.y = otherCollider.top + otherCollider.height + (m_BoundingBox.height / 2);
				}
				else if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::LEFT))
				{
					m_Position.x = otherCollider.left + otherCollider.width + (m_BoundingBox.width / 2);
					m_CanMoveLeft = false;
					m_CanMoveRight = true;
				}
				else if (collisionDirection & static_cast<int32_t>(Engine::CollisionDirection::RIGHT))
				{
					m_Position.x = otherCollider.left - (m_BoundingBox.width / 2);
					m_CanMoveLeft = true;
					m_CanMoveRight = false;
				}
			}
		}

		void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const 
		{
			if (!(m_HealthState == HealthState::DEAD))
			{
				target.draw(m_AnimationSprite);
			}
		}

		void Enemy::Move(float deltaTime) 
		{
			ActionType previousOrder = SeekASoul::Common::ToGameAction(m_PreviousAction);
			ActionType actionOrder = SeekASoul::Common::ToGameAction(m_CurrentAction);
			//LOG_DEBUG("ThreatLevel : " << static_cast<int>(GetThreatLevel()));
			//LOG_DEBUG("Current action : " << static_cast<int>(m_CurrentAction));
			//LOG_DEBUG("L : " << m_CanMoveLeft << " / R : " << m_CanMoveRight);
			sf::Vector2f tempVelocity(0.f, 0.f);

			// Apply gravity
			m_Velocity.y += GRAVITY; // put the gravity const in Entity
	
			// Configure velocity according to AI action order
			if (actionOrder == ActionType::NONE) 
			{
				m_Velocity.x = 0.f;
			}
			else 
			{
				if (actionOrder == ActionType::MOVE_RIGHT)
				{
					m_Velocity.x = MOVE_SPEED;
				}
				else if (actionOrder == ActionType::MOVE_LEFT)
				{
					m_Velocity.x = -MOVE_SPEED;
				}

				if (m_Strategy == AI::AIStrategyType::PATROL && actionOrder != previousOrder
					&& previousOrder != ActionType::NONE)
				{
					m_Velocity.y = -STAY_ON_PLATFORM_FORCE;
				}
			}
			
			// Check movement on X axis
			tempVelocity.x = m_Velocity.x;
			if (!GameManager::GetInstance()->CheckCollisions(this, tempVelocity * deltaTime).first)
			{
				m_Position += tempVelocity * deltaTime;
			}

			// Check movement on Y axis
			tempVelocity.x = 0.0f;
			tempVelocity.y = m_Velocity.y;
			if (!GameManager::GetInstance()->CheckCollisions(this, tempVelocity * deltaTime).first)
			{
				if (m_IsGrounded) 
				{
					m_IsGrounded = false;
					
					if (actionOrder == ActionType::MOVE_RIGHT)
					{
						m_CanMoveRight = false;
						m_CanMoveLeft = true;
					}
					else if (actionOrder == ActionType::MOVE_LEFT)
					{
						m_CanMoveRight = true;
						m_CanMoveLeft = false;
					}
				}

				m_Position += tempVelocity * deltaTime;
			}

			// Apply new position
			SetCenter(m_Position);
			SetAnimatedSpritePosition(m_Position);
		}

		void Enemy::Damage() 
		{
			LOG_INFO("Enemy was damaged !");

			m_AnimationSprite.setColor(sf::Color::Green);
			m_HealthState = HealthState::DAMAGED;
			m_HealthPoints -= static_cast<unsigned int>(Engine::Maths::GetRandom(5.f, 15.f));
	
			if (m_HealthPoints <= 0)
			{
				m_HealthState = HealthState::DEAD;
				LOG_INFO("Enemy died !");
			}

			m_LastDamageTime = Engine::Time::GetCurrentTimeAsMilliseconds();
		}

		void Enemy::UpdateVisualDamage(uint64_t now)
		{
			if (Engine::Maths::GetDifference(now, m_LastDamageTime) >= DAMAGE_COOLDOWN)
			{
				m_HealthState = HealthState::OK;
				m_AnimationSprite.setColor(sf::Color::White);
			}
		}
	}
}