#pragma once

#include "Wall.h"
#include "Bullet.h"
#include <memory>

class Player : public sf::Drawable, public BoxCollideable
{
public:	
	Player(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager);
	
	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	void InitColliders(const std::vector<Wall>& walls) { m_Walls = walls; };
	std::vector<Bullet>& GetBullets() { return m_Bullets; };

private:
	void UpdateShootingCooldown(float deltaTime);
	void UpdateBullets(float deltaTime);
	void Shoot();
	
	void ComputeVelocity();
	bool CheckCollision(const sf::Vector2f& nextPosition);
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

	// TODO : Use a better way to access collideable objects on the map
	std::vector<Wall> m_Walls; 
	std::vector<Bullet> m_Bullets;
};