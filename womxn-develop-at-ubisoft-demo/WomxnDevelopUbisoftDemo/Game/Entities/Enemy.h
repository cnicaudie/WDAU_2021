#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(const std::shared_ptr<TextureManager>& textureManager);

	void Update(float deltaTime) override;
	void OnCollision(const BoxCollideable* other) override;
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Damage(); // TODO : Move in Entity class ?

private:
	bool m_WasDamaged;
	float m_DamageCooldown;
};