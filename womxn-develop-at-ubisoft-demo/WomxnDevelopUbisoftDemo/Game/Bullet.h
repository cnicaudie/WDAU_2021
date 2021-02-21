#pragma once

class Bullet : public sf::Drawable, BoxCollideable {
public: 
	Bullet(const sf::Texture& texture, const sf::Vector2f& dir, const sf::Vector2f pos);
	~Bullet();
	
	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	inline float GetDistance() { return m_Distance; };

private:
	sf::Sprite m_Sprite;

	sf::Vector2f m_Direction;

	float m_Speed;

	float m_Distance;
};