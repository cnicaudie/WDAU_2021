#pragma once

#include "Entity.h"
#include <Game/Objects/Bullet.h>

enum PlayerState { IDLE, MOVING, SKULL_ROLLING, CLIMBING, SHOOTING };

class Player : public Entity, public Animated
{
public:	
	Player(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager);
	
	void Update(float deltaTime) override;
	void OnCollision(BoxCollideable* other) override;
	void OnTrigger(BoxCollideable* other) override;
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	inline const int GetNumberOfCollectedSoulChunks() const { return m_SoulChunksCollected; };
	inline const bool IsSkullRolling() const { return m_IsSkullRolling; };

protected:
	void Damage() override;
	void UpdateDamageCooldown(float deltaTime) override;

private:
	void UpdateBoundingBox();
	void UpdateShootingCooldown(float deltaTime);
	void UpdateSkullRollCooldown(float deltaTime);
	void ManageBullets(float deltaTime);
	void Shoot();
	
	void Move(float deltaTime);
	void ClampPlayerPosition(float minBoundX, float maxBoundX, float minBoundY, float maxBoundY);
	void ComputeNextPlayerState();

	//====================//

	std::shared_ptr<InputManager> m_InputManager;

	PlayerState m_CurrentState;
	
	std::vector<Bullet> m_Bullets;
	bool m_CanShoot;
	float m_ShootCooldown;
	int m_AmmunitionsNumber;

	int m_SoulChunksCollected;

	bool m_IsClimbing;

	bool m_IsSkullRolling;
	float m_SkullRollingCooldown;

	// Checks when gettin out of skull roll
	bool m_InGroundCollision;
	bool m_InCeilingCollision;
};