#pragma once

#include <set>
#include <Game/Action.h>
#include <Engine/Input/Binding.h>

class InputManager 
{
public:
	InputManager();

	void UpdateMousePosition(const sf::RenderWindow& gameWindow);

	void AddAction(const std::shared_ptr<Binding>& key);
	void RemoveAction(const std::shared_ptr<Binding>& key);

	// ==== Joystick

	/*void AddAction(sf::Joystick::Axis joystickAxis, unsigned int joystickIndex, float position);
	void RemoveAction(sf::Joystick::Axis joystickAxis, unsigned int joystickIndex, float position);

	void AddAction(unsigned int joystickIndex, unsigned int button);
	void RemoveAction(unsigned int joystickIndex, unsigned int button);*/

	// ==== 

	inline bool HasAction(Action action) const
	{
		return m_CurrentActions.find(action) != m_CurrentActions.end();
	}

	inline const sf::Vector2f GetMousePosition() const { return m_MousePosition; }

	inline const bool IsUsingJoystick() const { return m_IsUsingJoystick; }

	inline void SetJoystickIndex(unsigned int& index) { m_JoystickIndex = index; }

	inline void ResetJoystick() 
	{
		m_IsUsingJoystick = false;
		m_JoystickIndex = 0;
	}

private:
	sf::Vector2f m_MousePosition;
	std::set<Action> m_CurrentActions;

	//std::map<sf::Joystick::Axis, Action> m_JoystickBinding;
	//std::map<unsigned int, Action> m_JoystickButtonBinding;

	std::map<Action, std::vector<std::shared_ptr<Binding>>> m_ActionBinding;

	
	bool m_IsUsingJoystick;
	unsigned int m_JoystickIndex;
};