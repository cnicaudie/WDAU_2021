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
			LOG_INFO("InputManager Created");
		}

		InputManager::~InputManager() 
		{
			LOG_INFO("Destroyed InputManager!");
		}

		void InputManager::AddActionBinding(std::shared_ptr<Binding> binding, std::shared_ptr<IAction> action)
		{
			// TODO : Eventually make a rebinding feature
			m_ActionBinding.emplace(binding, action);
		}

		void InputManager::Update() 
		{
			for (std::shared_ptr<ActionEvent> actionEvent : m_CurrentActions)
			{
				EventManager::GetInstance()->Fire(actionEvent);
			}

			if (m_AimJoystickPosition.x != 0.f && m_AimJoystickPosition.y != 0.f)
			{
				std::shared_ptr<ActionEvent> actionEvent = std::make_shared<ActionEvent>(nullptr, 1.f, m_AimJoystickPosition, false, true);
				EventManager::GetInstance()->Fire(actionEvent);

				// And reset the aiming position
				m_AimJoystickPosition.x = 0.f;
				m_AimJoystickPosition.y = 0.f;
			}
		}

		void InputManager::RenderDebugMenu(sf::RenderTarget& target)
		{
			if (ImGui::CollapsingHeader("Mouse position"))
			{
				ImGui::Text("Game :");
				ImGui::SameLine();
				ImGui::Text("X: %f", m_GameMousePosition.x);
				ImGui::SameLine();
				ImGui::Text("Y: %f", m_GameMousePosition.y);
				ImGui::Text("GUI :");
				ImGui::SameLine();
				ImGui::Text("X: %f", m_GUIMousePosition.x);
				ImGui::SameLine();
				ImGui::Text("Y: %f", m_GUIMousePosition.y);
			}
		}

		void InputManager::ManageInputEvents(const sf::Event& event)
		{
			switch (event.type)
			{
				// === Keyboard 

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

				// === Mouse 

				case sf::Event::MouseButtonPressed:
				{
					AddAction(std::make_shared<MouseBinding>(event.mouseButton.button));

					// Fire event click (for GUI)
					std::shared_ptr<ClickEvent> clickEvent = std::make_shared<ClickEvent>(m_GUIMousePosition);
					EventManager::GetInstance()->Fire(clickEvent);
					break;
				}

				case sf::Event::MouseButtonReleased:
				{
					RemoveAction(std::make_shared<MouseBinding>(event.mouseButton.button));
					break;
				}

				case sf::Event::MouseMoved:
				{
					std::shared_ptr<ActionEvent> actionEvent = std::make_shared<ActionEvent>(nullptr, 1.f, m_GameMousePosition, true, true);
					EventManager::GetInstance()->Fire(actionEvent);
					break;
				}

				// === Joystick 

				case sf::Event::JoystickButtonPressed:
				{
					if (event.joystickButton.button < static_cast<unsigned int>(JoystickButton::MAX_VALUE))
					{
						AddAction(std::make_shared<JoystickButtonBinding>(static_cast<JoystickButton>(event.joystickButton.button)));
					}
					else 
					{
						LOG_WARNING("Button " << event.joystickButton.button << " is not supported.");
					}
					break;
				}

				case sf::Event::JoystickButtonReleased:
				{
					if (event.joystickButton.button < static_cast<unsigned int>(JoystickButton::MAX_VALUE))
					{
						RemoveAction(std::make_shared<JoystickButtonBinding>(static_cast<JoystickButton>(event.joystickButton.button)));
					}
					else
					{
						LOG_WARNING("Button " << event.joystickButton.button << " is not supported.");
					}
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
						AddAction(std::make_shared<JoystickAxisBinding>(joystickAxisBinding));
					}
					else
					{
						RemoveAction(std::make_shared<JoystickAxisBinding>(joystickAxisBinding));
					}

					break;
				}

				default:
					break;
			}
		}

		void InputManager::AddAction(std::shared_ptr<Binding> binding)
		{
			auto const& it = std::find_if(m_ActionBinding.begin(), m_ActionBinding.end()
				, [&](std::pair<std::shared_ptr<Binding>, std::shared_ptr<IAction>> actionBinding)
				{
					return *(actionBinding.first) == binding.get();
				});

			if (it != m_ActionBinding.end()) 
			{
				std::shared_ptr<IAction> action = it->second;
				auto const& actionPosition = std::find_if(m_CurrentActions.begin(), m_CurrentActions.end(), [&](const std::shared_ptr<ActionEvent> actionEvent)
					{
						return actionEvent->GetAction() == action.get();
					});
				const bool hasAction = m_CurrentActions.size() != 0 && actionPosition != m_CurrentActions.end();

				JoystickAxisBinding* joystickAxisBinding = dynamic_cast<JoystickAxisBinding*>(binding.get());

				// Handle actions other than related to a joystick axis input
				if (joystickAxisBinding == nullptr)
				{
					if (!hasAction)
					{
						std::shared_ptr<ActionEvent> actionEvent = std::make_shared<ActionEvent>(action.get());
						m_CurrentActions.push_back(actionEvent);
					}
				}
				// Manage joystick axis actions
				else {
					if (action->IsAimX())
					{
						m_AimJoystickPosition.x = joystickAxisBinding->GetAxisPosition();
					} 
					else if (action->IsAimY())
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
				
						std::shared_ptr<ActionEvent> actionEvent = std::make_shared<ActionEvent>(action.get(), actionScale);
						m_CurrentActions.push_back(actionEvent);
					}
				}
			}
		}

		void InputManager::RemoveAction(std::shared_ptr<Binding> binding)
		{
			auto const& it = std::find_if(m_ActionBinding.begin(), m_ActionBinding.end()
				, [&](std::pair<std::shared_ptr<Binding>, std::shared_ptr<IAction>> actionBinding)
				{
					return *(actionBinding.first) == binding.get();
				});

			if (it != m_ActionBinding.end())
			{
				std::shared_ptr<IAction> action = it->second;
		
				auto const& actionPosition = std::find_if(m_CurrentActions.begin(), m_CurrentActions.end(), [&](const std::shared_ptr<ActionEvent> actionEvent)
					{
						return actionEvent->GetAction() == action.get();
					});

				if (m_CurrentActions.size() != 0 && actionPosition != m_CurrentActions.end())
				{
					m_CurrentActions.erase(actionPosition);
				}
			}
		}

		bool InputManager::HasAction(IAction* action) const
		{
			auto it = std::find_if(m_CurrentActions.begin(), m_CurrentActions.end(), [&](const std::shared_ptr<ActionEvent> actionEvent)
				{
					return *(actionEvent->GetAction()) == action;
				});

			return it != m_CurrentActions.end();
		}
	}
}