#include <stdafx.h>
#include "InputManager.h"
#include <Engine/Input/Bindings/KeyboardBinding.h>
#include <Engine/Input/Bindings/MouseBinding.h>
#include <Engine/Input/Bindings/JoystickButtonBinding.h>

InputManager::InputManager() 
	: m_MousePosition()
	, m_IsUsingJoystick(false)
	, m_JoystickIndex(0)
	, m_JoystickDeadZone(5.0f)
{
	InitJoystick();

	// TODO : Make a default bindings file to parse at construction

	std::vector<std::shared_ptr<Binding>> moveUpBinding{ 
		std::make_shared<KeyboardBinding>(sf::Keyboard::Up),
		std::make_shared<KeyboardBinding>(sf::Keyboard::Z),
		std::make_shared<JoystickButtonBinding>(4) // L1 on PS3 Dualshock
	};
	m_ActionBinding.emplace(Action::MOVE_UP, moveUpBinding);
	
	std::vector<std::shared_ptr<Binding>> moveDownBinding{
		std::make_shared<KeyboardBinding>(sf::Keyboard::Down),
		std::make_shared<KeyboardBinding>(sf::Keyboard::S),
		std::make_shared<JoystickButtonBinding>(5) // R1 on PS3 Dualshock
	};
	m_ActionBinding.emplace(Action::MOVE_DOWN, moveDownBinding);

	std::vector<std::shared_ptr<Binding>> skullRollBinding{ 
		std::make_shared<KeyboardBinding>(sf::Keyboard::Space),
		std::make_shared<JoystickButtonBinding>(1) // O on PS3 Dualshock
	};
	m_ActionBinding.emplace(Action::SKULL_ROLL, skullRollBinding);

	std::vector<std::shared_ptr<Binding>> shootBinding{ 
		std::make_shared<MouseBinding>(sf::Mouse::Button::Right)
		// TODO : Manage shoot action with L2/R2 (=> manage axis)
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
			if (*b == key.get()) 
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
			if (*b == key.get())
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

const float InputManager::GetScaledVelocity(float currentVelocity, float speedInc, float maxSpeed, float slowdownRate) const
{
	float velocity = 0.f;

	if (m_IsUsingJoystick)
	{
		velocity = GetJoystickScaledAxis(m_JoystickIndex, sf::Joystick::Axis::X, maxSpeed);
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			velocity = fmin(currentVelocity + speedInc, maxSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
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

const sf::Vector2f InputManager::GetScaledShootDirection(const sf::Vector2f currentPosition) const
{
	sf::Vector2f shootDirection;

	if (m_IsUsingJoystick) 
	{
		const float xPos = GetJoystickScaledAxis(m_JoystickIndex, sf::Joystick::Axis::U, 1.f);
		const float yPos = GetJoystickScaledAxis(m_JoystickIndex, sf::Joystick::Axis::V, 1.f);
		shootDirection = sf::Vector2f(xPos, yPos);
	} 
	else 
	{
		const sf::Vector2f mousePos = m_MousePosition;
		shootDirection = mousePos - currentPosition;

		// TODO : Make a normalize function in a MathUtils file
		float magnitude = std::sqrt(shootDirection.x * shootDirection.x + shootDirection.y * shootDirection.y);
		shootDirection = shootDirection / magnitude;
	}

	return shootDirection;
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