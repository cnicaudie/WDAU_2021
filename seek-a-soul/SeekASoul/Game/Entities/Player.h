#pragma once

#include "Entity.h"
#include <Game/Objects/Bullet.h>

class Player : public Entity
{
public:	
	Player(const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<TextureManager>& textureManager);
	
	void Update(float deltaTime) override;
	void OnCollision(BoxCollideable* other) override;
	void OnTrigger(BoxCollideable* other) override;
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	inline const int GetNumberOfCollectedSoulChunks() { return m_SoulChunksCollected; };

protected:
	void Damage() override;

private:
	void UpdateShootingCooldown(float deltaTime);
	void UpdateBullets(float deltaTime);
	void Shoot();
	
	void Move(float deltaTime);
	void ClampPlayerPosition(float minBoundX, float maxBoundX, float minBoundY, float maxBoundY);

	//====================//

	std::shared_ptr<InputManager> m_InputManager;
	
	std::vector<Bullet> m_Bullets;
	bool m_CanShoot;
	float m_ShootCooldown;
	int m_AmmunitionsNumber;

	int m_SoulChunksCollected;
};