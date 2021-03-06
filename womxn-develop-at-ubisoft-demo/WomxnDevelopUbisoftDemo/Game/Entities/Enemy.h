#pragma once

class Enemy : public sf::Drawable, public BoxCollideable 
{
public:
	Enemy(const std::shared_ptr<TextureManager>& textureManager);

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void Damage();
	void OnCollision(const BoxCollideable* other) override;

	inline bool IsDead() const { return m_IsDead; };

private:
	sf::Sprite m_Sprite;
	sf::Vector2f m_Position;
	int m_HealthPoints;

	bool m_WasDamaged;
	float m_DamageCooldown;

	bool m_IsDead;
};