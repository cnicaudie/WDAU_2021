#include <stdafx.h>
#include "Button.h"

Button::Button(const sf::Vector2f& centerPosition, const sf::Vector2f& size)
	: m_WasClicked(false)
{
	m_ButtonRect.setSize(size);
	m_ButtonRect.setOrigin(size * 0.5f);
	m_ButtonRect.setPosition(centerPosition);
	m_ButtonRect.setFillColor(sf::Color::Magenta);
	m_ButtonRect.setOutlineThickness(5);
	m_ButtonRect.setOutlineColor(sf::Color::White);

	m_ButtonText.setCharacterSize(static_cast<unsigned int>(size.y / 2.f));
	m_ButtonText.setFillColor(sf::Color::White);
	m_ButtonText.setStyle(sf::Text::Bold);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_ButtonRect);
	target.draw(m_ButtonText);
};

void Button::OnEvent(const Event* evnt) 
{
	// OnEvent Mouse click => CheckClick(mousePos)
}