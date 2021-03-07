#include <stdafx.h>
#include "Door.h"


Door::Door(float xCenterPos, float yCenterPos, float width, float height)
	: m_IsPlayingEndGame{ false }
	, m_rColor{ 0.0f }
	, m_gColor{ 1.0f }
	, m_bColor{ 1.0f }
{
	const auto center = sf::Vector2f(xCenterPos, yCenterPos);
	const auto size = sf::Vector2f(width, height);
	SetTrigger(true);
	SetBoundingBox(center, size);

	m_Rectangle.setSize(size);
	m_Rectangle.setOrigin(size * 0.5f);
	m_Rectangle.setPosition(center);

	m_Rectangle.setFillColor(sf::Color(0, 0, 0, 0));
	m_Rectangle.setOutlineThickness(5);
	m_Rectangle.setOutlineColor(sf::Color{ static_cast<uint8_t>(m_rColor * 255.0f), static_cast<uint8_t>(m_gColor * 255.0f), static_cast<uint8_t>(m_bColor * 255.0f) });
}

Door::~Door()
{
	std::cout << "Destroyed Door" << std::endl;
}

void Door::Update(float deltaTime)
{
	if (!m_IsPlayingEndGame)
	{
		return;
	}

	float colorDelta = 0.2f * deltaTime /1000.0f;

	m_rColor = fmodf(m_rColor+colorDelta, 1.f);
	m_gColor = fmodf(m_gColor+colorDelta, 1.f);
	m_bColor = fmodf(m_bColor+colorDelta, 1.f);
	m_Rectangle.setOutlineColor(sf::Color{ static_cast<uint8_t>(m_rColor * 255.0f), static_cast<uint8_t>(m_gColor * 255.0f), static_cast<uint8_t>(m_bColor * 255.0f) });
}

void Door::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Rectangle);
}

void Door::OnTrigger(const BoxCollideable* other)
{
	if (typeid(*other) == typeid(class Player) && !m_IsPlayingEndGame)
	{
		std::cout << "Walk through door !" << std::endl;
		StartEndGame();
		GameManager::GetInstance()->StartEndGame();
	}
}

void Door::StartEndGame()
{
	m_IsPlayingEndGame = true;

	m_rColor = 0.25f;
	m_gColor = 0.5f;
	m_bColor = 0.75f;
	m_Rectangle.setOutlineColor(sf::Color{ static_cast<uint8_t>(m_rColor * 255.0f), static_cast<uint8_t>(m_gColor * 255.0f), static_cast<uint8_t>(m_bColor * 255.0f) });
}