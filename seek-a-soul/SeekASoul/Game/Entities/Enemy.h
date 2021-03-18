#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(const std::shared_ptr<TextureManager>& textureManager);

	void Update(float deltaTime) override;
	void OnCollision(BoxCollideable* other) override;
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	void Damage() override;
	virtual void UpdateDamageCooldown(float deltaTime) override;

private:
	sf::Sprite m_Sprite;

};