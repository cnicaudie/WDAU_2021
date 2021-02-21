#pragma once

#include <Game/Wall.h>
#include <Game/Bullet.h>
#include <vector>

class MainCharacter : public sf::Drawable, public BoxCollideable
{
public:	
	MainCharacter();
	
	void Update(float deltaTime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void StartEndGame();

	inline void InitColliders(const std::vector<Wall>& walls) { m_Walls = walls; };

private:
	void ComputeVelocity();
	void Move(float deltaTime);
	bool CheckCollision(const sf::Vector2f& nextPosition);

	sf::Texture m_Texture;
	sf::Texture m_BulletTexture;
	sf::Sprite m_Sprite;

	bool m_IsUsingJoystick;
	unsigned int m_JoystickIndex;
	bool m_WasButtonPressed;

	sf::Vector2f m_Position; 
	sf::Vector2f m_Velocity;
	bool m_IsPlayingEndGame;

	bool m_IsGrounded;
	bool m_IsJumping;
	bool m_canShoot;
	float m_shootCooldown;

	std::vector<Wall> m_Walls;
	std::vector<Bullet> m_Bullets;
};