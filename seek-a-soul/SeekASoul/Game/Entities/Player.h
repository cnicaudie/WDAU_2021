#pragma once

#include "Entity.h"
#include <Engine/Animation/Animated.h>
#include <Game/Objects/Bone.h>
#include <AI/Threat/Threat.h>

namespace SeekASoul
{
	namespace UI
	{
		class UIViewModel;
	}

	namespace Gameplay
	{
		class MovingPlatform;
		
		class Player : public Entity, public Engine::Animated, public AI::Threat
		{
		public:	
			Player(const std::shared_ptr<Engine::InputManager>& inputManager, const std::shared_ptr<Engine::TextureManager>& textureManager);
			~Player();

			void Reset(const sf::Vector2f& position, bool restart);
	
			void Update(float deltaTime) override;
			void OnCollision(Engine::BoxCollideable* other, Engine::CollisionDirection direction) override;
			void OnTrigger(Engine::BoxCollideable* other) override;
	
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
			void RenderDebugMenu(sf::RenderTarget& target);
	
			inline const int GetNumberOfCollectedSoulChunks() const { return m_SoulChunksCollected; };
			inline const bool IsAttacking() const { return m_IsSkullRolling; };

			virtual const bool operator==(Threat* other) const override
			{ 
				return dynamic_cast<Player*>(other) != nullptr;
			}

		protected:
			void Damage() override;
			void UpdateVisualDamage(uint64_t now) override;

		private:
			void OnEvent(const Engine::Event* evnt);
			void ComputeNextPlayerState();
			void Move(float deltaTime);
			void MoveUp();
			void MoveDown();
			void MoveRight(const float scale);
			void MoveLeft(const float scale);
			void ClampPlayerPosition(float minBoundX, float maxBoundX, float minBoundY, float maxBoundY);
			void ApplyCollisionCorrection(const int32_t& collisionDirection, sf::FloatRect& otherCollider);
	
			void UpdateBoundingBox();
			void UpdateThrowDirection(const sf::Vector2f& direction, const bool isPoint);
			void ThrowBone();
			void SkullRoll();
			void UpdateSkullRollCooldown(uint64_t now);
			void ManageBones(float deltaTime);
			void Die();
	
			//====================//

			std::shared_ptr<Engine::InputManager> m_InputManager;

			enum class PlayerState 
			{ 
				IDLE			= 0, 
				MOVING			= 1, 
				SKULL_ROLLING	= 2, 
				CLIMBING		= 3, 
				THROWING		= 4
			} m_CurrentState;
	
			unsigned int m_SoulChunksCollected;
			unsigned int m_JumpCount;
			bool m_IsClimbing; // TODO : Use player state instead
			bool m_CanClimb;
			bool m_IsSkullRolling; // TODO : Use player state instead
			uint64_t m_LastSkullRollTime;
			uint64_t m_LastThrowTime;

			sf::Vector2f m_ThrowDirection;
			std::vector<Bone> m_Bones;
			bool m_InfiniteAmmos;
			unsigned int m_AmmunitionsNumber;

			// Checks when gettin out of skull roll (change of bounding box)
			bool m_InGroundCollision;
			bool m_InCeilingCollision;

			bool m_IsOnMovingPlatform;
			MovingPlatform* m_Platform;
		};
	}
}