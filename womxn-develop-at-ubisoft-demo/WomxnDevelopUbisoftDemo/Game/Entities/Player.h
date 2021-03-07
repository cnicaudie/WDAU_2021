#pragma once

#include "Entity.h"
#include <Game/Objects/Bullet.h>

class Player : public Entity
{
public:	
	Player(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager);
	
	void Update(float deltaTime) override;
	void OnCollision(const BoxCollideable* other) override;
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	std::vector<Bullet>& GetBullets() { return m_Bullets; };

	
private:
	void UpdateShootingCooldown(float deltaTime);
	void UpdateBullets(float deltaTime);
	void Shoot();
	
	void ComputeVelocity();
	void Move(float deltaTime);

	//====================//

	std::shared_ptr<InputManager> m_InputManager;
	
	// TODO : Manage input in an Input manager
	unsigned int m_JoystickIndex;
	
	bool m_CanShoot;
	float m_ShootCooldown;
	int m_AmmunitionsNumber;

	std::vector<Bullet> m_Bullets;
};