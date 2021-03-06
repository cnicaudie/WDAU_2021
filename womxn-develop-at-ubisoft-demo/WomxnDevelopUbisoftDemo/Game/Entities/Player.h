#pragma once

#include <Game/Objects/Bullet.h>

class Player : public sf::Drawable, public BoxCollideable
{
public:	
	Player(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager);
	
	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void OnCollision(const std::shared_ptr<BoxCollideable>& other) override;

	std::vector<Bullet>& GetBullets() { return m_Bullets; };

	inline const sf::Vector2f GetVelocity() { return m_Velocity; }; // For debug in GameManager

private:
	void UpdateShootingCooldown(float deltaTime);
	void UpdateBullets(float deltaTime);
	void Shoot();
	
	void ComputeVelocity();
	void Move(float deltaTime);

	//====================//

	std::shared_ptr<InputManager> m_InputManager;
	std::shared_ptr<TextureManager> m_TextureManager;
	sf::Sprite m_Sprite;

	// TODO : Manage input in an Input manager
	unsigned int m_JoystickIndex;
	
	sf::Vector2f m_Position; 
	sf::Vector2f m_Velocity;
	
	bool m_IsGrounded;

	bool m_CanShoot;
	float m_ShootCooldown;
	int m_AmmunitionsNumber;

	std::vector<Bullet> m_Bullets;
};