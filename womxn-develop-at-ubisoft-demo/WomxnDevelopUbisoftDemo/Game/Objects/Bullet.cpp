#include <stdafx.h>

Bullet::Bullet(const std::shared_ptr<TextureManager>& textureManager, const sf::Vector2f& dir, const sf::Vector2f pos)
	: m_Direction(dir), m_Speed(500.f), m_Distance(0.f), m_HadImpact(false)
{
	sf::Vector2f textureSize = textureManager->GetTextureSizeFromName("BULLET");
	
	m_Sprite.setTexture(textureManager->GetTextureFromName("BULLET"));
	m_Sprite.setOrigin(textureSize * 0.5f);
	m_Sprite.setPosition(pos);
	m_Sprite.scale(sf::Vector2f(0.25f, 0.25f));
	
	SetBoundingBox(pos, textureSize * 0.25f);
	SetTrigger(false);
	std::cout << "Bullet created !" << std::endl;
}

Bullet::~Bullet() {
	std::cout << "Bullet destroyed !" << std::endl;
}

void Bullet::Update(float deltaTime) 
{
	sf::Vector2f positionOffset(m_Direction * m_Speed * deltaTime);

	m_HadImpact = GameManager::GetInstance()->CheckCollision(this, positionOffset);
	
	m_Sprite.move(positionOffset);

	float offsetX = std::abs(GetCenter().x - m_Sprite.getPosition().x);
	float offsetY = std::abs(GetCenter().y - m_Sprite.getPosition().y);
	m_Distance += std::sqrt(offsetX * offsetX + offsetY * offsetY);

	SetCenter(m_Sprite.getPosition());
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Sprite);
}

void Bullet::OnCollision(const BoxCollideable* other)
{
	std::cout << "Bullet collided with : " << typeid(*other).name() << std::endl;
}