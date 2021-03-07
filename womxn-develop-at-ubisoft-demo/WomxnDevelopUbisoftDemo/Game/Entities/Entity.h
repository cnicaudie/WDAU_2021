#pragma once

class Entity : public sf::Drawable, public BoxCollideable 
{
public:
	Entity(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& position, int healthPoints);
	
	virtual void Update(float deltaTime) = 0;
	virtual void OnCollision(const BoxCollideable* other) = 0;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	inline const sf::Vector2f GetVelocity() { return m_Velocity; }; // For debug in GameManager
	inline bool IsDead() const { return m_IsDead; };

protected:
	std::shared_ptr<TextureManager> m_TextureManager;
	sf::Sprite m_Sprite;
	
	sf::Vector2f m_Position;
	sf::Vector2f m_Velocity;
	bool m_IsGrounded;

	int m_HealthPoints;
	bool m_IsDead;
};