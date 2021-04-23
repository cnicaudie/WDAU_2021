#include <stdafx.h>
#include "Button.h"
#include <Engine/Event/EventTypes/ClickEvent.h>

Button::Button(const sf::Vector2f& size)
	: m_WasClicked(false)
{
	m_ButtonRect.setSize(size);
	m_ButtonRect.setOrigin(size * 0.5f);
	m_ButtonRect.setFillColor(sf::Color::Blue);
	m_ButtonRect.setOutlineThickness(5);
	m_ButtonRect.setOutlineColor(sf::Color::White);

	m_ButtonText.setCharacterSize(static_cast<unsigned int>(size.y / 2.f));
	m_ButtonText.setFillColor(sf::Color::White);
	m_ButtonText.setStyle(sf::Text::Bold);

	EventListener<Button, ClickEvent> listener(this, &Button::OnEvent);
	EventManager::GetInstance()->AddListener(listener);
}

Button::~Button() 
{
	EventListener<Button, ClickEvent> listener(this, &Button::OnEvent);
	EventManager::GetInstance()->RemoveListener(listener);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_ButtonRect);
	target.draw(m_ButtonText);
};

void Button::OnEvent(const Event* evnt) 
{
	if (const ClickEvent* clickEvent = dynamic_cast<const ClickEvent*>(evnt))
	{
		CheckClick(clickEvent->GetClickPosition());
	}
}