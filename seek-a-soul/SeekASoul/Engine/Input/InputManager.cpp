#include <stdafx.h>
#include "InputManager.h"
#include <Engine/Input/Bindings/KeyboardBinding.h>
#include <Engine/Input/Bindings/MouseBinding.h>
#include <Engine/Input/Bindings/JoystickButtonBinding.h>
#include <Engine/Input/Bindings/JoystickAxisBinding.h>

InputManager::InputManager()
	: m_MousePosition()
	, m_IsUsingJoystick(false)
	, m_JoystickIndex(0)
	, m_JoystickDeadZone(30.0f)
{
	InitJoystick();

	// TODO : Make a default bindings file to parse at construction

	std::vector<std::shared_ptr<Binding>> moveUpBinding{ 
		std::make_shared<KeyboardBinding>(sf::Keyboard::Up),
		std::make_shared<KeyboardBinding>(sf::Keyboard::Z),
		std::make_shared<JoystickButtonBinding>(5) // R1 on PS3 Dualshock
	};
	m_ActionBinding.emplace(Action::MOVE_UP, moveUpBinding);
	
	std::vector<std::shared_ptr<Binding>> moveDownBinding{
		std::make_shared<KeyboardBinding>(sf::Keyboard::Down),
		std::make_shared<KeyboardBinding>(sf::Keyboard::S),
		std::make_shared<JoystickButtonBinding>(4) // L1 on PS3 Dualshock
	};
	m_ActionBinding.emplace(Action::MOVE_DOWN, moveDownBinding);

	std::vector<std::shared_ptr<Binding>> skullRollBinding{ 
		std::make_shared<KeyboardBinding>(sf::Keyboard::Space),
		std::make_shared<JoystickButtonBinding>(1) // O on PS3 Dualshock
	};
	m_ActionBinding.emplace(Action::SKULL_ROLL, skullRollBinding);

	std::vector<std::shared_ptr<Binding>> shootBinding{ 
		std::make_shared<MouseBinding>(sf::Mouse::Button::Right),
		std::make_shared<JoystickAxisBinding>(sf::Joystick::Axis::Z, true) // L2 on PS3 Dualshock
	};
	m_ActionBinding.emplace(Action::SHOOT, shootBinding);

	// TODO : Eventually make a rebinding feature

	std::cout << "InputManager Created" << std::endl;
}

void InputManager::ManageInputEvents(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::KeyPressed:
		{
			AddAction(std::make_shared<KeyboardBinding>(event.key.code));
			break;
		}

		case sf::Event::KeyReleased:
		{
			RemoveAction(std::make_shared<KeyboardBinding>(event.key.code));
			break;
		}

		case sf::Event::MouseButtonPressed:
		{
			AddAction(std::make_shared<MouseBinding>(event.mouseButton.button));
			break;
		}

		case sf::Event::MouseButtonReleased:
		{
			RemoveAction(std::make_shared<MouseBinding>(event.mouseButton.button));
			break;
		}

		case sf::Event::JoystickButtonPressed:
		{
			AddAction(std::make_shared<JoystickButtonBinding>(event.joystickButton.button));
			break;
		}

		case sf::Event::JoystickButtonReleased:
		{
			RemoveAction(std::make_shared<JoystickButtonBinding>(event.joystickButton.button));
			break;
		}

		case sf::Event::JoystickMoved:
		{
			// Min threshold to consider a "press" on the the axis
			const float PRESSED_THRESHOLD = 60.f;

			float joystickPosition = event.joystickMove.position;
			sf::Joystick::Axis joystickAxis = event.joystickMove.axis;

			if (joystickAxis == sf::Joystick::Axis::Z) // Only axis used for actions for now
			{
				if (joystickPosition >= PRESSED_THRESHOLD || joystickPosition <= -PRESSED_THRESHOLD)
				{
					AddAction(std::make_shared<JoystickAxisBinding>(joystickAxis, joystickPosition > 0));
				}
				else
				{
					RemoveAction(std::make_shared<JoystickAxisBinding>(joystickAxis, joystickPosition > 0));
				}
			}
			break;
		}

		case sf::Event::JoystickConnected:
		{
			if (!IsUsingJoystick())
			{
				InitJoystick();
			}
			break;
		}

		case sf::Event::JoystickDisconnected:
		{
			ResetJoystick(event.joystickConnect.joystickId);
			break;
		}

		default:
			break;
	}
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
		const float xPos = sf::Joystick::getAxisPosition(m_JoystickIndex, sf::Joystick::Axis::U);
		const float yPos = sf::Joystick::getAxisPosition(m_JoystickIndex, sf::Joystick::Axis::V);
		shootDirection = sf::Vector2f(xPos, yPos);
	} 
	else 
	{
		const sf::Vector2f mousePos = m_MousePosition;
		shootDirection = mousePos - currentPosition;
	}

	// Normalize the vector
	// TODO : Make a normalize function in a MathUtils file
	float magnitude = std::sqrt(shootDirection.x * shootDirection.x + shootDirection.y * shootDirection.y);
	shootDirection = shootDirection / magnitude;

	return shootDirection;
}

void InputManager::InitJoystick()
{
	unsigned int index = 0;
	while (index < sf::Joystick::Count)
	{
		if (sf::Joystick::isConnected(index) && sf::Joystick::hasAxis(index, sf::Joystick::Axis::X) && sf::Joystick::hasAxis(index, sf::Joystick::Axis::Y))
		{
			m_IsUsingJoystick = true;
			m_JoystickIndex = index;
			return;
		}

		index++;
	}
}

const float InputManager::GetJoystickScaledAxis(unsigned int index, sf::Joystick::Axis axis, float scale) const
{
	float value = sf::Joystick::getAxisPosition(index, axis);

	if (value >= -m_JoystickDeadZone && value <= m_JoystickDeadZone) 
	{
		return 0.0f;
	}

	value = (value / 100.0f) * scale;

	return value;
}