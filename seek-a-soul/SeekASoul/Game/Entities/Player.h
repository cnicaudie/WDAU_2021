#pragma once

#include "Entity.h"
#include <Game/Objects/Bullet.h>

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

	enum class PlayerState 
	{ 
		IDLE			= 0, 
		MOVING			= 1, 
		SKULL_ROLLING	= 2, 
		CLIMBING		= 3, 
		SHOOTING		= 4
	} m_CurrentState;
	
	int m_SoulChunksCollected;

	std::vector<Bullet> m_Bullets;
	bool m_CanShoot;
	float m_ShootCooldown;
	int m_AmmunitionsNumber;

	// Checks when gettin out of skull roll
	bool m_InGroundCollision;
	bool m_InCeilingCollision;
};