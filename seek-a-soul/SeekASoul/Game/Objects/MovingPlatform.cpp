#include <stdafx.h>
#include "MovingPlatform.h"
#include <Engine/Maths/Maths.h>

namespace SeekASoul
{
	namespace Gameplay
	{
		MovingPlatform::MovingPlatform(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition, const sf::Vector2f& size)
			: m_Direction(endPosition - startPosition)
			, m_Velocity(80.f)
		{
			Engine::Maths::NormalizeVector(m_Direction);

			InitMoveBox(startPosition, endPosition, size);
			InitPlatform(size, startPosition);

			SetBoundingBox(startPosition, size);
			SetTrigger(false);
		}

		void MovingPlatform::InitMoveBox(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition, const sf::Vector2f& size)
		{
			const float width = std::abs(endPosition.x - startPosition.x) == 0 ? size.x : std::abs(endPosition.x - startPosition.x);
			const float height = std::abs(endPosition.y - startPosition.y) == 0 ? size.y : std::abs(endPosition.y - startPosition.y);
			const sf::Vector2f widthHeight{ width, height };

			const float top = startPosition.y <= endPosition.y ? startPosition.y : endPosition.y;
			const float left = startPosition.x <= endPosition.x ? startPosition.x : endPosition.x;
			const sf::Vector2f topLeft{ left, top };

			m_MoveBox.setSize(widthHeight);
			m_MoveBox.setPosition(topLeft);

			// For debug purposes
			m_MoveBox.setFillColor(sf::Color::Transparent);
			m_MoveBox.setOutlineThickness(1);
			m_MoveBox.setOutlineColor(sf::Color::Red);
		}

		void MovingPlatform::InitPlatform(const sf::Vector2f& size, const sf::Vector2f& startPosition)
		{
			const sf::FloatRect moveBox = m_MoveBox.getGlobalBounds();
			const float top = moveBox.top;
			const float bottom = moveBox.top + moveBox.height;
			const float left = moveBox.left;
			const float right = moveBox.left + moveBox.width;

			m_PlatformRect.setSize(size);
			m_PlatformRect.setOrigin(size * 0.5f);
			m_PlatformRect.setFillColor(sf::Color::White);

			sf::Vector2f fixedStartPosition = startPosition;
			
			if (Engine::Maths::Equals(startPosition, { left, bottom }, 1.f))
			{
				fixedStartPosition.y -= size.y;
			}
			else if (Engine::Maths::Equals(startPosition, { right, top }, 1.f))
			{
				fixedStartPosition.x -= size.x;
			}
			else if (Engine::Maths::Equals(startPosition, { right, bottom }, 1.f))
			{
				fixedStartPosition.x -= size.x;
				fixedStartPosition.y -= size.y;
			}

			m_PlatformRect.setPosition({ fixedStartPosition.x + size.x / 2.f, fixedStartPosition.y + size.y / 2.f });
		}

		void MovingPlatform::Update(float deltaTime)
		{
			// Move the platform
			const sf::Vector2f positionOffset(m_Direction * m_Velocity * deltaTime);
	
			m_PlatformRect.move(positionOffset);
			SetCenter(m_PlatformRect.getPosition());

			// Check if the platform went out of its movement zone
			const sf::FloatRect moveBox = m_MoveBox.getGlobalBounds();
			const sf::FloatRect platformBox = m_PlatformRect.getGlobalBounds();

			if (!(moveBox.contains(platformBox.left, platformBox.top)
				&& moveBox.contains(platformBox.left + platformBox.width, platformBox.top + platformBox.height)))
			{
				m_Velocity = -m_Velocity;
			}
		}

		void MovingPlatform::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(m_PlatformRect);
			// Uncomment next line to debug the move box
			// TODO : Add a checkbox to toggle it in the debug menu
			//target.draw(m_MoveBox);
		}
	}
}