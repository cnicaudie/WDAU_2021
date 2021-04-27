#include <stdafx.h>
#include "InputManager.h"
#include <Engine/Input/Bindings/KeyboardBinding.h>
#include <Engine/Input/Bindings/MouseBinding.h>
#include <Engine/Input/Bindings/JoystickButtonBinding.h>
#include <Engine/Input/Bindings/JoystickAxisBinding.h>
#include <Engine/Event/EventTypes/ActionEvent.h>
#include <Engine/Event/EventTypes/ClickEvent.h>

namespace SeekASoul
{
	namespace Engine
	{
		static constexpr float JOYSTICK_DEAD_ZONE = 20.f;

		InputManager::InputManager()
		{
			// TODO : Make a default bindings file to parse at construction

			m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Up), Gameplay::Action::MOVE_UP);
			m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Z), Gameplay::Action::MOVE_UP);
			m_ActionBinding.emplace(new JoystickButtonBinding(5), Gameplay::Action::MOVE_UP); // R1 on PS3 Dualshock

			m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Down), Gameplay::Action::MOVE_DOWN);
			m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::S), Gameplay::Action::MOVE_DOWN);
			m_ActionBinding.emplace(new JoystickButtonBinding(4), Gameplay::Action::MOVE_DOWN); // L1 on PS3 Dualshock
	
			m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Right), Gameplay::Action::MOVE_RIGHT);
			m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::D), Gameplay::Action::MOVE_RIGHT);
			m_ActionBinding.emplace(new JoystickAxisBinding(sf::Joystick::Axis::X, true), Gameplay::Action::MOVE_RIGHT); // Left joystick on PS3 Dualshock

			m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Left), Gameplay::Action::MOVE_LEFT);
			m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Q), Gameplay::Action::MOVE_LEFT);
			m_ActionBinding.emplace(new JoystickAxisBinding(sf::Joystick::Axis::X, false), Gameplay::Action::MOVE_LEFT); // Left joystick on PS3 Dualshock

			m_ActionBinding.emplace(new KeyboardBinding(sf::Keyboard::Space), Gameplay::Action::SKULL_ROLL);
			m_ActionBinding.emplace(new JoystickButtonBinding(1), Gameplay::Action::SKULL_ROLL); // O on PS3 Dualshock

			m_ActionBinding.emplace(new MouseBinding(sf::Mouse::Button::Right), Gameplay::Action::SHOOT);
			m_ActionBinding.emplace(new JoystickAxisBinding(sf::Joystick::Axis::Z, true), Gameplay::Action::SHOOT); // L2 on PS3 Dualshock

			m_ActionBinding.emplace(new JoystickAxisBinding(sf::Joystick::Axis::U, true), Gameplay::Action::AIM_X); // Right joystick on PS3 Dualshock
			m_ActionBinding.emplace(new JoystickAxisBinding(sf::Joystick::Axis::U, false), Gameplay::Action::AIM_X); // Right joystick on PS3 Dualshock
	
			m_ActionBinding.emplace(new JoystickAxisBinding(sf::Joystick::Axis::V, true), Gameplay::Action::AIM_Y); // Right joystick on PS3 Dualshock
			m_ActionBinding.emplace(new JoystickAxisBinding(sf::Joystick::Axis::V, false), Gameplay::Action::AIM_Y); // Right joystick on PS3 Dualshock

			// TODO : Eventually make a rebinding feature

			LOG_INFO("InputManager Created");
		}

		InputManager::~InputManager() 
		{
			for (std::pair<Binding*, Gameplay::Action> actionBinding : m_ActionBinding)
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

			if (m_AimJoystickPosition.x != 0.f && m_AimJoystickPosition.y != 0.f)
			{
				std::shared_ptr<ActionEvent> actionEvent = std::make_shared<ActionEvent>(Gameplay::Action::AIM, 1.f, m_AimJoystickPosition, false);
				EventManager::GetInstance()->Fire(actionEvent);

				// And reset the aiming position
				m_AimJoystickPosition.x = 0.f;
				m_AimJoystickPosition.y = 0.f;
			}
		}

		void InputManager::ManageInputEvents(const sf::Event& event)
		{
			switch (event.type)
			{
				// === Keyboard 

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

				// === Mouse 

				case sf::Event::MouseButtonPressed:
				{
					MouseBinding mouseBinding(event.mouseButton.button);
					AddAction(&mouseBinding);

					// Fire event click (for GUI)
					std::shared_ptr<ClickEvent> clickEvent = std::make_shared<ClickEvent>(m_GUIMousePosition);
					EventManager::GetInstance()->Fire(clickEvent);
					break;
				}

				case sf::Event::MouseButtonReleased:
				{
					MouseBinding mouseBinding(event.mouseButton.button);
					RemoveAction(&mouseBinding);
					break;
				}

				case sf::Event::MouseMoved:
				{
					std::shared_ptr<ActionEvent> actionEvent = std::make_shared<ActionEvent>(Gameplay::Action::AIM, 1.f, m_GameMousePosition, true);
					EventManager::GetInstance()->Fire(actionEvent);
					break;
				}

				// === Joystick 

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
					// TODO : Manage artifacts ?
					float joystickPosition = event.joystickMove.position;
					sf::Joystick::Axis joystickAxis = event.joystickMove.axis;
					JoystickAxisBinding joystickAxisBinding(joystickAxis, joystickPosition > 0, joystickPosition);

					if (joystickPosition >= JOYSTICK_DEAD_ZONE || joystickPosition <= -JOYSTICK_DEAD_ZONE)
					{
						AddAction(&joystickAxisBinding);
					}
					else
					{
						RemoveAction(&joystickAxisBinding);
					}

					break;
				}

				default:
					break;
			}
		}

		void InputManager::AddAction(Binding* key)
		{
			auto const& it = std::find_if(m_ActionBinding.begin(), m_ActionBinding.end(), [&](std::pair<Binding*, Gameplay::Action> binding)
				{
					return *(binding.first) == key;
				});

			if (it != m_ActionBinding.end()) 
			{
				Gameplay::Action action = it->second;
				auto const& actionPosition = std::find_if(m_CurrentActions.begin(), m_CurrentActions.end(), [&](const std::shared_ptr<ActionEvent> actionEvent)
					{
						return actionEvent->GetActionType() == action;
					});
				const bool hasAction = m_CurrentActions.size() != 0 && actionPosition != m_CurrentActions.end();

				JoystickAxisBinding* joystickAxisBinding = dynamic_cast<JoystickAxisBinding*>(key);

				// Handle actions other than related to a joystick axis input
				if (joystickAxisBinding == nullptr)
				{
					if (!hasAction)
					{
						std::shared_ptr<ActionEvent> actionEvent = std::make_shared<ActionEvent>(action);
						m_CurrentActions.push_back(actionEvent);
					}
				}
				// Manage joystick axis actions
				else {
					if (action == Gameplay::Action::AIM_X)
					{
						m_AimJoystickPosition.x = joystickAxisBinding->GetAxisPosition();
					} 
					else if (action == Gameplay::Action::AIM_Y)
					{
						m_AimJoystickPosition.y = joystickAxisBinding->GetAxisPosition();
					} 
					else
					{
						float actionScale = std::abs(joystickAxisBinding->GetAxisPosition()) / 100.f;

						// We want to replace the existing action (position update)
						if (hasAction)
						{
							m_CurrentActions.erase(actionPosition);
						} 
				
						std::shared_ptr<ActionEvent> actionEvent = std::make_shared<ActionEvent>(action, actionScale);
						m_CurrentActions.push_back(actionEvent);
					}
				}
			}
		}

		void InputManager::RemoveAction(Binding* key)
		{
			auto const& it = std::find_if(m_ActionBinding.begin(), m_ActionBinding.end(), [&](std::pair<Binding*, Gameplay::Action> binding)
				{
					return *(binding.first) == key;
				});

			if (it != m_ActionBinding.end())
			{
				Gameplay::Action action = it->second;
		
				auto const& actionPosition = std::find_if(m_CurrentActions.begin(), m_CurrentActions.end(), [&](const std::shared_ptr<ActionEvent> actionEvent)
					{
						return actionEvent->GetActionType() == action;
					});

				if (m_CurrentActions.size() != 0 && actionPosition != m_CurrentActions.end())
				{
					m_CurrentActions.erase(actionPosition);
				}
			}
		}
	}
}