#pragma once

#include "Entity.h"
#include <Engine/Animation/Animated.h>
#include <AI/AIEntity.h>

namespace SeekASoul
{
	namespace Gameplay
	{
		class Enemy : public Entity, public Engine::Animated, public AI::AIEntity
		{
		public:
			Enemy(const std::shared_ptr<Engine::TextureManager>& textureManager, const sf::Vector2f& position);
			~Enemy();

			void Update(float deltaTime) override;
			void OnCollision(Engine::BoxCollideable* other, Engine::CollisionDirection direction) override;
	
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		protected:
			void Damage() override;
			void UpdateVisualDamage(uint64_t now) override;

		private:
			void Move(float deltaTime);
		};
	}
}