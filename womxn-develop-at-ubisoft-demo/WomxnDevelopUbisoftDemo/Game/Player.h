#pragma once

#include "Bullet.h"
#include <memory>

class Player : public sf::Drawable, public BoxCollideable
{
public:	
	Player(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager);
	
	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	std::vector<Bullet>& GetBullets() { return m_Bullets; };

	inline void SetGroundLevel(bool isGrounded) { m_IsGrounded = isGrounded; };

	inline void SetPositionX(float x) { m_Position.x = x; }
	inline void SetPositionY(float y) { m_Position.y = y; }

	inline void ResetVelocityX() { m_Velocity.x = 0.f; };
	inline void ResetVelocityY() { m_Velocity.y = 0.f; };

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
	bool m_IsUsingJoystick;
	unsigned int m_JoystickIndex;
	bool m_WasButtonPressed;

	sf::Vector2f m_Position; 
	sf::Vector2f m_Velocity;
	
	bool m_IsGrounded;

	bool m_CanShoot;
	float m_ShootCooldown;
	int m_AmmunitionsNumber;

	std::vector<Bullet> m_Bullets;
};