#include <stdafx.h>
#include "InputManager.h"
#include <Engine/Input/Bindings/KeyboardBinding.h>
#include <Engine/Input/Bindings/MouseBinding.h>
#include <Engine/Input/Bindings/JoystickButtonBinding.h>
#include <Engine/Input/Bindings/JoystickAxisBinding.h>
#include <Engine/Event/EventTypes/ActionEvent.h>

InputManager::InputManager()
	: m_MousePosition()
	, m_IsUsingJoystick(false)
	, m_JoystickIndex(0)
	, m_JoystickDeadZone(30.0f)
{
	InitJoystick();

	// TODO : Make a default bindings file to parse at construction

	m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Up), Action::MOVE_UP);
	m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Z), Action::MOVE_UP);
	m_ActionBinding.emplace(new JoystickButtonBinding(5), Action::MOVE_UP); // R1 on PS3 Dualshock

	m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Down), Action::MOVE_DOWN);
	m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::S), Action::MOVE_DOWN);
	m_ActionBinding.emplace(new JoystickButtonBinding(4), Action::MOVE_DOWN); // L1 on PS3 Dualshock
	
	m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Right), Action::MOVE_RIGHT);
	m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::D), Action::MOVE_RIGHT);
	m_ActionBinding.emplace(new JoystickAxisBinding(sf::Joystick::Axis::X, true), Action::MOVE_RIGHT); // Left joystick on PS3 Dualshock

	m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Left), Action::MOVE_LEFT);
	m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Q), Action::MOVE_LEFT);
	m_ActionBinding.emplace(new JoystickAxisBinding(sf::Joystick::Axis::X, false), Action::MOVE_LEFT); // Left joystick on PS3 Dualshock

	m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Space), Action::SKULL_ROLL);
	m_ActionBinding.emplace(new JoystickButtonBinding(1), Action::SKULL_ROLL); // O on PS3 Dualshock

	m_ActionBinding.emplace(new MouseBinding(sf::Mouse::Button::Right), Action::SHOOT);
	m_ActionBinding.emplace(new JoystickAxisBinding(sf::Joystick::Axis::Z, true), Action::SHOOT); // L2 on PS3 Dualshock

	// TODO : Eventually make a rebinding feature

	LOG_INFO("InputManager Created");
}

InputManager::~InputManager() 
{
	for (std::pair<Binding*, Action> actionBinding : m_ActionBinding) 
	{
		delete actionBinding.first;
	}
}

void InputManager::Update() 
{
	for (std::shared_ptr<ActionEvent> actionEvent : m_CurrentActions)
	{
		EventManager::GetInstance()->Fire(actionEvent);
	}
}

void InputManager::ManageInputEvents(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::KeyPressed:
		{
			KeyboardBinding keyboardBinding(event.key.code);
			AddAction(&keyboardBinding);
			break;
		}

		case sf::Event::KeyReleased:
		{
			KeyboardBinding keyboardBinding(event.key.code);
			RemoveAction(&keyboardBinding);
			break;
		}

		case sf::Event::MouseButtonPressed:
		{
			MouseBinding mouseBinding(event.mouseButton.button);
			AddAction(&mouseBinding);
			break;
		}

		case sf::Event::MouseButtonReleased:
		{
			MouseBinding mouseBinding(event.mouseButton.button);
			RemoveAction(&mouseBinding);
			break;
		}

		case sf::Event::JoystickButtonPressed:
		{
			JoystickButtonBinding joystickButtonBinding(event.joystickButton.button);
			AddAction(&joystickButtonBinding);
			break;
		}

		case sf::Event::JoystickButtonReleased:
		{
			JoystickButtonBinding joystickButtonBinding(event.joystickButton.button);
			RemoveAction(&joystickButtonBinding);
			break;
		}

		case sf::Event::JoystickMoved:
		{
			float joystickPosition = event.joystickMove.position;
			sf::Joystick::Axis joystickAxis = event.joystickMove.axis;

			if (joystickPosition >= m_JoystickDeadZone || joystickPosition <= -m_JoystickDeadZone)
			{
				JoystickAxisBinding joystickAxisBinding(joystickAxis, joystickPosition > 0, joystickPosition);
				AddAction(&joystickAxisBinding);
			}
			else
			{
				JoystickAxisBinding joystickAxisBinding(joystickAxis, joystickPosition > 0, joystickPosition);
				RemoveAction(&joystickAxisBinding);
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

void InputManager::AddAction(Binding* key)
{
	auto const& it = std::find_if(m_ActionBinding.begin(), m_ActionBinding.end(), [&](std::pair<Binding*, Action> binding)
		{
			return *(binding.first) == key;
		});

	if (it != m_ActionBinding.end()) 
	{
		Action action = it->second;
		
		if (!HasAction(action)) 
		{
			if (JoystickAxisBinding* joystickAxisBinding = dynamic_cast<JoystickAxisBinding*>(key)) 
			{
				std::shared_ptr<ActionEvent> actionEvent = std::make_shared<ActionEvent>(action, joystickAxisBinding->GetAxisPosition() / 100);
				m_CurrentActions.push_back(actionEvent);
			} 
			else 
			{
				std::shared_ptr<ActionEvent> actionEvent = std::make_shared<ActionEvent>(action, 1.f);
				m_CurrentActions.push_back(actionEvent);
			}
		}
	}
}

void InputManager::RemoveAction(Binding* key)
{
	auto const& it = std::find_if(m_ActionBinding.begin(), m_ActionBinding.end(), [&](std::pair<Binding*, Action> binding)
		{
			return *(binding.first) == key;
		});

	if (it != m_ActionBinding.end())
	{
		Action action = it->second;

		auto const& pos = std::find_if(m_CurrentActions.begin(), m_CurrentActions.end(), [&](const std::shared_ptr<ActionEvent> actionEvent)
			{
				return actionEvent->GetActionType() == action;
			});

		if (pos != m_CurrentActions.end())
		{
			m_CurrentActions.erase(pos);
		}
	}
}

void InputManager::UpdateMousePosition(const sf::RenderWindow& gameWindow)
{
	const sf::Vector2i& mousePixelPosition = sf::Mouse::getPosition(gameWindow);
	m_MousePosition = gameWindow.mapPixelToCoords(mousePixelPosition);
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
		if (sf::Joystick::isConnected(index) 
			&& sf::Joystick::hasAxis(index, sf::Joystick::Axis::X) 
			&& sf::Joystick::hasAxis(index, sf::Joystick::Axis::Y))
		{
			m_IsUsingJoystick = true;
			m_JoystickIndex = index;
			return;
		}

		index++;
	}
}