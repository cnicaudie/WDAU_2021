#pragma once

#include <set>
#include <Game/Action.h>
#include <Engine/Input/Bindings/Binding.h>
#include <Engine/Event/EventTypes/ActionEvent.h>

class InputManager 
{
public:
	InputManager();
	~InputManager();

	void Update();
	void ManageInputEvents(const sf::Event& event);

	void AddAction(Binding* key);
	void RemoveAction(Binding* key);

	inline bool HasAction(Action action) const
	{
		auto it = std::find_if(m_CurrentActions.begin(), m_CurrentActions.end(), [&](const std::shared_ptr<ActionEvent> actionEvent)
			{
				return actionEvent->GetActionType() == action;
			});
		
		return it != m_CurrentActions.end();
	}

	void UpdateMousePosition(const sf::RenderWindow& gameWindow);
	const sf::Vector2f GetScaledShootDirection(const sf::Vector2f currentPosition) const;

	void InitJoystick();

	inline void ResetJoystick(unsigned int joystickIndex)
	{
		if (m_JoystickIndex == joystickIndex) 
		{
			m_IsUsingJoystick = false;
			m_JoystickIndex = 0;
		}
	};
	
	inline const bool IsUsingJoystick() const { return m_IsUsingJoystick; };
	inline const float GetJoystickDeadZone() const { return m_JoystickDeadZone; };
	inline const sf::Vector2f GetMousePosition() const { return m_MousePosition; };

private:
	std::vector<std::shared_ptr<ActionEvent>> m_CurrentActions;
	std::map<Binding*, Action> m_ActionBinding;

	sf::Vector2f m_MousePosition;
	sf::Vector2f m_AimJoystickPosition;

	bool m_IsUsingJoystick;
	unsigned int m_JoystickIndex;
	float m_JoystickDeadZone;
};