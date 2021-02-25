#include <stdafx.h>
#include "InputManager.h"

InputManager::InputManager() 
	: m_MousePosition()
	, m_IsUsingJoystick(false)
	, m_JoystickIndex(0)
{
	// Jump action
	m_KeyboardBinding.emplace(sf::Keyboard::Up, Action::JUMP);

	// Shoot action
	m_MouseBinding.emplace(sf::Mouse::Button::Right, Action::SHOOT);

	// Squeeze action 
	m_KeyboardBinding.emplace(sf::Keyboard::Space, Action::SQUEEZE);
	m_JoystickButtonBinding.emplace(0, Action::SQUEEZE);


	// TODO : How to know which button is which on the controller ? 
	// TODO : Make test to find the right controls
	// TODO : Eventually make a rebinding feature
	//m_JoystickButtonBinding.emplace(1, Action::JUMP);

	std::cout << "InputManager Created" << std::endl;
}

void InputManager::UpdateMousePosition(const sf::RenderWindow& gameWindow)
{
	const sf::Vector2i& mousePixelPosition = sf::Mouse::getPosition(gameWindow);
	m_MousePosition = gameWindow.mapPixelToCoords(mousePixelPosition);
}

// ==== Keyboard 

void InputManager::AddAction(sf::Keyboard::Key key)
{
	if (m_KeyboardBinding.find(key) != m_KeyboardBinding.end())
	{
		m_CurrentActions.insert(m_KeyboardBinding.at(key));
	}
}

void InputManager::RemoveAction(sf::Keyboard::Key key)
{
	if (m_KeyboardBinding.find(key) != m_KeyboardBinding.end())
	{
		auto pos = m_CurrentActions.find(m_KeyboardBinding.at(key));
		if (pos != m_CurrentActions.end())
		{
			m_CurrentActions.erase(pos);
		}
	}
}

// ==== Mouse 

void InputManager::AddAction(sf::Mouse::Button button)
{
	if (m_MouseBinding.find(button) != m_MouseBinding.end())
	{
		m_CurrentActions.insert(m_MouseBinding.at(button));
	}
}

void InputManager::RemoveAction(sf::Mouse::Button button)
{
	if (m_MouseBinding.find(button) != m_MouseBinding.end())
	{
		auto pos = m_CurrentActions.find(m_MouseBinding.at(button));
		if (pos != m_CurrentActions.end())
		{
			m_CurrentActions.erase(pos);
		}
	}
}

// ==== Joystick

// TODO : Adapt using elements in Player class
void InputManager::AddAction(sf::Joystick::Axis joystickAxis, unsigned int joystickIndex, float position)
{
	if (m_JoystickBinding.find(joystickAxis) != m_JoystickBinding.end())
	{
		m_CurrentActions.insert(m_JoystickBinding.at(joystickAxis));
	}
}

// TODO : See how to check if joystick isn't moving
void InputManager::RemoveAction(sf::Joystick::Axis joystickAxis, unsigned int joystickIndex, float position)
{
	if (m_JoystickBinding.find(joystickAxis) != m_JoystickBinding.end())
	{
		auto pos = m_CurrentActions.find(m_JoystickBinding.at(joystickAxis));
		if (pos != m_CurrentActions.end())
		{
			m_CurrentActions.erase(pos);
		}
	}
}

void InputManager::AddAction(unsigned int joystickIndex, unsigned int button)
{
	if (m_JoystickIndex == joystickIndex) 
	{
		if (m_JoystickButtonBinding.find(button) != m_JoystickButtonBinding.end())
		{
			m_CurrentActions.insert(m_JoystickButtonBinding.at(button));
		}
	}
}

void InputManager::RemoveAction(unsigned int joystickIndex, unsigned int button)
{
	if (m_JoystickIndex == joystickIndex)
	{
		if (m_JoystickButtonBinding.find(button) != m_JoystickButtonBinding.end())
		{
			auto pos = m_CurrentActions.find(m_JoystickButtonBinding.at(button));
			if (pos != m_CurrentActions.end())
			{
				m_CurrentActions.erase(pos);
			}
		}
	}
}