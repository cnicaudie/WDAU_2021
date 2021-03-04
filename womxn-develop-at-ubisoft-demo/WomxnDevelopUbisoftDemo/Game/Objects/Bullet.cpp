#include <stdafx.h>
#include "Bullet.h"
#include <cmath>

Bullet::Bullet(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& dir, const sf::Vector2f pos)
	: m_Direction(dir), m_Speed(500.f), m_Distance(0.f), m_HadImpact(false)
{
	sf::Vector2f textureSize = textureManager->GetTextureSizeFromName("BULLET");
	
	m_Sprite.setTexture(textureManager->GetTextureFromName("BULLET"));
	m_Sprite.setOrigin(textureSize * 0.5f);
	m_Sprite.setPosition(pos);
	m_Sprite.scale(sf::Vector2f(0.5f, 0.5f));
	
	SetBoundingBox(pos, textureSize);
	std::cout << "Bullet created !" << std::endl;
}

Bullet::~Bullet() {
	std::cout << "Bullet destroyed !" << std::endl;
}

void Bullet::Update(float deltaTime) 
{
	m_HadImpact = GameManager::GetInstance()->CheckBulletImpact(*this);

	float oldPosX = GetCenter().x;
	sf::Vector2f newPos(m_Direction * m_Speed * deltaTime);
	m_Sprite.move(newPos);
	
	SetCenter(m_Sprite.getPosition());
	m_Distance += std::abs(oldPosX - m_Sprite.getPosition().x);
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Sprite);
}