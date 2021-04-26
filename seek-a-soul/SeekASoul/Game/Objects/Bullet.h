#pragma once

class Bullet : public sf::Drawable, public BoxCollideable {
public: 
	Bullet(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& dir, const sf::Vector2f pos);
	~Bullet();
	
	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void OnCollision(BoxCollideable* other, CollisionDirection direction) override;

	inline const float GetDistanceTraveled() const { return m_TraveledDistance; };
	inline const bool HadImpact() const { return m_HadImpact; };

private:
	sf::Sprite m_Sprite;

	sf::Vector2f m_Direction;
	float m_Velocity;
	float m_TraveledDistance;
	
	bool m_HadImpact;
};