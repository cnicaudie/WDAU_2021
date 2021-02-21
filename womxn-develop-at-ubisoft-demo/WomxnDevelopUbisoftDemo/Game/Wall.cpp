#include "stdafx.h"
#include "Wall.h"

Wall::Wall(float xCenterPos, float yCenterPos, float width, float height)
{
	const auto center = sf::Vector2f(xCenterPos, yCenterPos);
	const auto size = sf::Vector2f(width, height);
	SetBoundingBox(center, size);

	m_Rectangle.setSize(size);
	m_Rectangle.setOrigin(size * 0.5f);
	m_Rectangle.setPosition(center);

	m_Rectangle.setFillColor(sf::Color::White);
}

Wall::~Wall()
{
}

void Wall::Update(float deltaTime)
{
}

void Wall::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Rectangle);
}