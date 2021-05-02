#pragma once

namespace SeekASoul
{
	namespace Gameplay
	{
		class MovingPlatform : public sf::Drawable, public Engine::BoxCollideable
		{
		public:
			MovingPlatform(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition, const sf::Vector2f& size);
			~MovingPlatform() = default;

			void Update(float deltaTime);
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

			inline const sf::Vector2f GetPlatformOffset() const { return m_Velocity * m_Direction; };
		
			//====================//

			static bool DisplayMoveBox;

		private:
			void InitMoveBox(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition, const sf::Vector2f& size);
			void InitPlatform(const sf::Vector2f& size, const sf::Vector2f& startPosition);
			
			//====================//

			sf::RectangleShape m_PlatformRect;
			sf::RectangleShape m_MoveBox;
	
			sf::Vector2f m_Direction;
			float m_Velocity;
		};
	}
}