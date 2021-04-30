#include <stdafx.h>
#include "Bone.h"

namespace SeekASoul
{
	namespace Gameplay
	{
		static const sf::Vector2i BONE_SPRITE_SIZE{ 32, 32 };

		Bone::Bone(const std::shared_ptr<Engine::TextureManager>& textureManager, const sf::Vector2f& dir, const sf::Vector2f pos)
			: Animated(BONE_SPRITE_SIZE, textureManager->GetTextureFromName("BONE_SHEET"))
			, m_Direction(dir)
			, m_Velocity(500.f)
			, m_TraveledDistance(0.f)
			, m_HadImpact(false)
		{
			SetAnimatedSpritePosition(pos);
			SetBoundingBox(pos, static_cast<sf::Vector2f>(BONE_SPRITE_SIZE));
			SetTrigger(false);
		}

		Bone::~Bone() {}

		void Bone::Update(float deltaTime) 
		{
			PlayAnimation(0, 0.05f);

			// Compute offset to next position and check for any collision
			sf::Vector2f positionOffset(m_Direction * m_Velocity * deltaTime);
			GameManager::GetInstance()->CheckCollisions(this, positionOffset);
	
			m_AnimationSprite.move(positionOffset);

			// Compute traveled distance
			float offsetX = std::abs(GetCenter().x - m_AnimationSprite.getPosition().x);
			float offsetY = std::abs(GetCenter().y - m_AnimationSprite.getPosition().y);
			m_TraveledDistance += std::sqrt(offsetX * offsetX + offsetY * offsetY);

			SetCenter(m_AnimationSprite.getPosition());
		}

		void Bone::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(m_AnimationSprite);
		}

		void Bone::OnCollision(Engine::BoxCollideable* other, Engine::CollisionDirection direction)
		{
			if (!m_HadImpact) 
			{
				LOG_INFO("Bone had impact with : " << typeid(*other).name());
				m_HadImpact = true;
			}
		}
	}
}