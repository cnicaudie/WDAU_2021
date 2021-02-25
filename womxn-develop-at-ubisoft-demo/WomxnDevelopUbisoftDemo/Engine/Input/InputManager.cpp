#include <stdafx.h>
#include "InputManager.h"
#include <Engine/Input/KeyboardBinding.h>
#include <Engine/Input/MouseBinding.h>


InputManager::InputManager() 
	: m_MousePosition()
	, m_IsUsingJoystick(false)
	, m_JoystickIndex(0)
{
	std::vector<std::shared_ptr<Binding>> jumpBinding{ std::make_shared<KeyboardBinding>(sf::Keyboard::Up) };
	m_ActionBinding.emplace(Action::JUMP, jumpBinding);
	
	std::vector<std::shared_ptr<Binding>> squeezeBinding{ std::make_shared<KeyboardBinding>(sf::Keyboard::Space) };
	m_ActionBinding.emplace(Action::SQUEEZE, squeezeBinding);

	std::vector<std::shared_ptr<Binding>> shootBinding{ std::make_shared<MouseBinding>(sf::Mouse::Button::Right) };
	m_ActionBinding.emplace(Action::SHOOT, shootBinding);
	

	// TODO : How to know which button is which on the controller ? 
	// TODO : Make test to find the right controls
	// TODO : Eventually make a rebinding feature
	//m_JoystickButtonBinding.emplace(0, Action::SQUEEZE);
	//m_JoystickButtonBinding.emplace(1, Action::JUMP);

	std::cout << "InputManager Created" << std::endl;
}

void InputManager::UpdateMousePosition(const sf::RenderWindow& gameWindow)
{
	const sf::Vector2i& mousePixelPosition = sf::Mouse::getPosition(gameWindow);
	m_MousePosition = gameWindow.mapPixelToCoords(mousePixelPosition);
}

void InputManager::AddAction(const std::shared_ptr<Binding>& key)
{
	for (auto it = m_ActionBinding.begin(); it != m_ActionBinding.end(); it++)
	{
		for (std::shared_ptr<Binding>& b : it->second)
		{
			if (b->GetBinding() == key->GetBinding()) 
			{
				m_CurrentActions.insert(it->first);
				return;
			}
		}
	}
}

void InputManager::RemoveAction(const std::shared_ptr<Binding>& key)
{
	for (auto it = m_ActionBinding.begin(); it != m_ActionBinding.end(); it++)
	{
		for (std::shared_ptr<Binding>& b : it->second)
		{
			if (b->GetBinding() == key->GetBinding())
			{
				auto pos = m_CurrentActions.find(it->first);
				if (pos != m_CurrentActions.end())
				{
					m_CurrentActions.erase(pos);
				}
				return;
			}
		}
	}
}


// ==== Joystick

/*// TODO : Adapt using elements in Player class
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
}*/