#pragma once

class Enemy : public sf::Drawable, BoxCollideable 
{
public:
	Enemy();

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	int m_HealthPoints;
};