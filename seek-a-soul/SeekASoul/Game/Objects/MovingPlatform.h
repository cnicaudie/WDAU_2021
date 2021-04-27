#pragma once

namespace SeekASoul
{
	namespace Gameplay
	{
		class MovingPlatform : public sf::Drawable, public Engine::BoxCollideable
		{
		public:
			MovingPlatform(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition, const sf::Vector2f& size);
			~MovingPlatform();

			void Update(float deltaTime);
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

			inline const sf::Vector2f GetPlatformOffset() const { return m_Velocity * m_Direction; };

		private:
			sf::RectangleShape m_PlatformRect;
			//sf::FloatRect m_MoveBox;
			sf::RectangleShape m_MoveBox;
	
			sf::Vector2f m_Direction;
			float m_Velocity;
		};
	}
}