#pragma once

class Enemy : public sf::Drawable, public BoxCollideable 
{
public:
	Enemy();

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Damage();

private:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	sf::Vector2f m_Position;
	int m_HealthPoints;

	bool m_wasDamaged;
	float m_damageCooldown;
};