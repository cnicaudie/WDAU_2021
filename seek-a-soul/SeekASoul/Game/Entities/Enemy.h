#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& position);

	void Update(float deltaTime) override;
	void OnCollision(BoxCollideable* other, CollisionDirection direction) override;
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	void Damage() override;
	void UpdateVisualDamage(uint64_t now) override;

private:
	sf::Sprite m_Sprite;

};