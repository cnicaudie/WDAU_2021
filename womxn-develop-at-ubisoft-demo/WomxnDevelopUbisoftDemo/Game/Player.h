#pragma once

#include "Wall.h"
#include "Bullet.h"

class Player : public sf::Drawable, public BoxCollideable
{
public:	
	Player();
	
	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	void InitColliders(const std::vector<Wall>& walls) { m_Walls = walls; };
	const std::vector<Bullet> GetBullets() { return m_Bullets; };

private:
	//========== Methods ==========//

	// Shooting system
	void UpdateShootingCooldown(float deltaTime);
	void UpdateBullets(float deltaTime);
	void Shoot();

	// Movement management
	void ComputeVelocity();
	bool CheckCollision(const sf::Vector2f& nextPosition);
	void Move(float deltaTime);

	//========== Attributes ==========//

	// TODO : Move all textures in a texture manager
	sf::Texture m_Texture;
	sf::Texture m_BulletTexture;
	sf::Sprite m_Sprite;

	// TODO : Manage input in an Input manager
	bool m_IsUsingJoystick;
	unsigned int m_JoystickIndex;
	bool m_WasButtonPressed;

	// Movement and position
	sf::Vector2f m_Position; 
	sf::Vector2f m_Velocity;
	
	bool m_IsGrounded;

	// Shooting system
	bool m_canShoot;
	float m_shootCooldown;
	int m_AmmunitionsNumber;

	std::vector<Wall> m_Walls;
	std::vector<Bullet> m_Bullets;
};