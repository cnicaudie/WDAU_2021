#pragma once

#include "Entity.h"
#include <Game/Objects/Bullet.h>

class Player : public Entity, public Animated
{
public:	
	Player(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager);
	
	void Update(float deltaTime) override;
	void OnEvent(const Event* evnt);
	void OnCollision(BoxCollideable* other, CollisionDirection direction) override;
	void OnTrigger(BoxCollideable* other) override;
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	inline const int GetNumberOfCollectedSoulChunks() const { return m_SoulChunksCollected; };
	inline const bool IsSkullRolling() const { return m_IsSkullRolling; };

protected:
	void Damage() override;
	void UpdateVisualDamage(uint64_t now) override;

private:
	void ComputeNextPlayerState();
	void Move(float deltaTime);
	void MoveUp();
	void MoveDown();
	void MoveRight(const float scale);
	void MoveLeft(const float scale);
	void ClampPlayerPosition(float minBoundX, float maxBoundX, float minBoundY, float maxBoundY);
	
	void UpdateBoundingBox();
	void Shoot();
	void SkullRoll();
	void UpdateSkullRollCooldown(uint64_t now);
	void ManageBullets(float deltaTime);
	
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
	int m_JumpCount;
	bool m_IsGrounded;
	bool m_IsClimbing; // TODO : Use player state instead
	bool m_CanClimb;
	bool m_IsSkullRolling; // TODO : Use player state instead
	uint64_t m_LastSkullRollTime;
	uint64_t m_LastShootTime;

	std::vector<Bullet> m_Bullets;
	int m_AmmunitionsNumber;

	// Checks when gettin out of skull roll (change of bounding box)
	bool m_InGroundCollision;
	bool m_InCeilingCollision;
};