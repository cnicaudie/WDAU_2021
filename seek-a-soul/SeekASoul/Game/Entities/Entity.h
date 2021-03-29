#pragma once

class Entity : public sf::Drawable, public BoxCollideable
{
public:
	Entity(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& position, int healthPoints);
	
	virtual void Update(float deltaTime) = 0;
	virtual void OnCollision(BoxCollideable* other, CollisionDirection direction) = 0;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

	inline const sf::Vector2f& GetVelocity() const { return m_Velocity; }; // For debug in GameManager
	inline bool IsDead() const { return m_HealthState == HealthState::DEAD; };

protected:
	virtual void Damage() = 0;
	virtual void UpdateVisualDamage(uint64_t deltaTime) = 0;

	//====================//
	
	enum class HealthState
	{
		OK		= 0,
		DAMAGED	= 1,
		DEAD	= 2,
	} m_HealthState;

	std::shared_ptr<TextureManager> m_TextureManager;
	
	sf::Vector2f m_Position;
	sf::Vector2f m_Velocity;
	
	int m_HealthPoints;

	uint64_t m_LastDamageTime;
};