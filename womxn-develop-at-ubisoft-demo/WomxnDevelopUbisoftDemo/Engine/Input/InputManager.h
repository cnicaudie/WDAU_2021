#pragma once

#include <set>
#include <Game/Action.h>
#include <Engine/Input/Binding.h>

class InputManager 
{
public:
	InputManager();

	void AddAction(const std::shared_ptr<Binding>& key);
	void RemoveAction(const std::shared_ptr<Binding>& key);

	inline bool HasAction(Action action) const
	{
		return m_CurrentActions.find(action) != m_CurrentActions.end();
	}

	void UpdateMousePosition(const sf::RenderWindow& gameWindow);

	inline const sf::Vector2f GetMousePosition() const { return m_MousePosition; }

	inline const bool IsUsingJoystick() const { return m_IsUsingJoystick; }

	inline float GetJoystickScaledAxis(unsigned int index, sf::Joystick::Axis axis, float deadZone, float scale)
	{
		float value = (sf::Joystick::getAxisPosition(index, axis) / 100.0f) * scale;
		if (value >= -deadZone && value <= deadZone)
		{
			return 0.0f;
		}

		return value;
	}

private:
	void InitJoystick();

	std::set<Action> m_CurrentActions;
	std::map<Action, std::vector<std::shared_ptr<Binding>>> m_ActionBinding;

	sf::Vector2f m_MousePosition;
	bool m_IsUsingJoystick;
	unsigned int m_JoystickIndex;
};