#include <stdafx.h>
#include "InputManager.h"
#include <Engine/Input/Bindings/KeyboardBinding.h>
#include <Engine/Input/Bindings/MouseBinding.h>
#include <Engine/Input/Bindings/JoystickButtonBinding.h>

InputManager::InputManager() 
	: m_MousePosition()
	, m_IsUsingJoystick(false)
	, m_JoystickIndex(0)
{
	InitJoystick();

	// TODO : Make a default bindings file to parse at construction
	std::vector<std::shared_ptr<Binding>> moveUpBinding{ 
		std::make_shared<KeyboardBinding>(sf::Keyboard::Up),
		std::make_shared<KeyboardBinding>(sf::Keyboard::Z),
		std::make_shared<JoystickButtonBinding>(0)
	};
	m_ActionBinding.emplace(Action::MOVE_UP, moveUpBinding);
	
	std::vector<std::shared_ptr<Binding>> moveDownBinding{
		std::make_shared<KeyboardBinding>(sf::Keyboard::Down),
		std::make_shared<KeyboardBinding>(sf::Keyboard::S)
		// TODO : Manage joystick
	};
	m_ActionBinding.emplace(Action::MOVE_DOWN, moveDownBinding);

	std::vector<std::shared_ptr<Binding>> squeezeBinding{ 
		std::make_shared<KeyboardBinding>(sf::Keyboard::Space),
		std::make_shared<JoystickButtonBinding>(1)
	};
	m_ActionBinding.emplace(Action::SQUEEZE, squeezeBinding);

	std::vector<std::shared_ptr<Binding>> shootBinding{ 
		std::make_shared<MouseBinding>(sf::Mouse::Button::Right)
		// TODO : Manage shoot direction with joystick direction ?
	};
	m_ActionBinding.emplace(Action::SHOOT, shootBinding);

	// TODO : Eventually make a rebinding feature

	std::cout << "InputManager Created" << std::endl;
}

void InputManager::AddAction(const std::shared_ptr<Binding>& key)
{
	for (auto it = m_ActionBinding.begin(); it != m_ActionBinding.end(); it++)
	{
		for (std::shared_ptr<Binding>& b : it->second)
		{
			if (typeid(*key).name() == typeid(*b).name() && b->GetBinding() == key->GetBinding()) 
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
			if (typeid(*key).name() == typeid(*b).name() && b->GetBinding() == key->GetBinding())
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

void InputManager::UpdateMousePosition(const sf::RenderWindow& gameWindow)
{
	const sf::Vector2i& mousePixelPosition = sf::Mouse::getPosition(gameWindow);
	m_MousePosition = gameWindow.mapPixelToCoords(mousePixelPosition);
}

const float InputManager::GetScaledVelocity(float currentVelocity, float speedInc, float maxSpeed, float slowdownRate, float deadZone) const
{
	float velocity = 0.f;

	if (m_IsUsingJoystick)
	{
		velocity = GetJoystickScaledAxis(m_JoystickIndex, sf::Joystick::Axis::X, deadZone, maxSpeed);
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			velocity = fmin(currentVelocity + speedInc, maxSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			velocity = fmax(currentVelocity - speedInc, -maxSpeed);
		}
		else
		{
			velocity = currentVelocity * slowdownRate;
		}
	}

	return velocity;
}

void InputManager::InitJoystick()
{
	unsigned int index = 0;
	while (index < sf::Joystick::Count)
	{
		if (sf::Joystick::isConnected(index) && sf::Joystick::hasAxis(index, sf::Joystick::Axis::X) && sf::Joystick::hasAxis(index, sf::Joystick::Axis::Y))
		{
			std::cout << "Joystick connected!" << std::endl;
			m_IsUsingJoystick = true;
			m_JoystickIndex = index;
			return;
		}

		index++;
	}
}