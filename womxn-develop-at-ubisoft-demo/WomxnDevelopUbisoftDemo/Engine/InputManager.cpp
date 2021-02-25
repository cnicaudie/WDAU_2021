#include <stdafx.h>
#include "InputManager.h"

InputManager::InputManager() 
	: m_MousePosition()
{
	//m_keyboardBind.emplace("Jump", )
	std::cout << "InputManager Created" << std::endl;
}

void InputManager::UpdateMousePosition(const sf::RenderWindow& gameWindow)
{
	const sf::Vector2i& mousePixelPosition = sf::Mouse::getPosition(gameWindow);
	m_MousePosition = gameWindow.mapPixelToCoords(mousePixelPosition);
}