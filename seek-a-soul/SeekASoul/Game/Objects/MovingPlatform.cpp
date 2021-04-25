#include <stdafx.h>
#include <Engine/Maths/Maths.h>
#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(const sf::Vector2f& startPosition, const sf::Vector2f& endPosition, const sf::Vector2f& size)
	: m_Direction(endPosition - startPosition)
	, m_Velocity(80.f)
{
	Maths::NormalizeVector(m_Direction);

	// Init platform rect
	m_PlatformRect.setSize(size);
	m_PlatformRect.setOrigin(size * 0.5f);
	m_PlatformRect.setPosition({ startPosition.x + size.x / 2.f, startPosition.y + size.y / 2.f });

	m_PlatformRect.setFillColor(sf::Color::White);

	// Init move box
	float width = std::abs(endPosition.x - startPosition.x) == 0 ? size.x : std::abs(endPosition.x - startPosition.x);
	float height = std::abs(endPosition.y - startPosition.y) == 0 ? size.y : std::abs(endPosition.y - startPosition.y);
	sf::Vector2f widthHeight{ width, height };
	
	float top = startPosition.y <= endPosition.y ? startPosition.y : endPosition.y;
	float left = startPosition.x <= endPosition.x ? startPosition.x : endPosition.x;
	sf::Vector2f center{ left + width/2.f, top + height/2.f };

	//m_MoveBox = sf::FloatRect{ topLeft, size };
	m_MoveBox.setSize(widthHeight);
	m_MoveBox.setOrigin(widthHeight * 0.5f);
	m_MoveBox.setPosition(center);

	m_MoveBox.setFillColor(sf::Color::Transparent);
	m_MoveBox.setOutlineThickness(1);
	m_MoveBox.setOutlineColor(sf::Color::Red);

	SetBoundingBox(startPosition, size);
	SetTrigger(false);
}

MovingPlatform::~MovingPlatform() {}

void MovingPlatform::Update(float deltaTime)
{
	// Move the platform
	sf::Vector2f positionOffset(m_Direction * m_Velocity * deltaTime);
	
	m_PlatformRect.move(positionOffset);
	SetCenter(m_PlatformRect.getPosition());

	const sf::FloatRect platformBox = m_PlatformRect.getGlobalBounds();

	if (!(m_MoveBox.getGlobalBounds().contains(platformBox.left, platformBox.top)
		&& m_MoveBox.getGlobalBounds().contains(platformBox.left + platformBox.width, platformBox.top + platformBox.height)))
	{
		m_Velocity = -m_Velocity;
	}
}

void MovingPlatform::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_PlatformRect);
	target.draw(m_MoveBox);
}