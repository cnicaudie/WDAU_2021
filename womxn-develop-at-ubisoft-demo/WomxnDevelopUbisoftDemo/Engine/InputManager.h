#pragma once

#include <set>
#include <Game/Action.h>

class InputManager 
{
public:
	InputManager();

	void UpdateMousePosition(const sf::RenderWindow& gameWindow);

	// ==== Keyboard 

	void AddAction(sf::Keyboard::Key key);
	void RemoveAction(sf::Keyboard::Key key);

	// ==== Mouse 

	void AddAction(sf::Mouse::Button button);
	void RemoveAction(sf::Mouse::Button button);

	// ==== Joystick

	void AddAction(sf::Joystick::Axis joystickAxis, unsigned int joystickIndex, float position);
	void RemoveAction(sf::Joystick::Axis joystickAxis, unsigned int joystickIndex, float position);

	void AddAction(unsigned int joystickIndex, unsigned int button);
	void RemoveAction(unsigned int joystickIndex, unsigned int button);

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

	std::map<sf::Keyboard::Key, Action> m_KeyboardBinding;
	std::map<sf::Mouse::Button, Action> m_MouseBinding;

	std::map<sf::Joystick::Axis, Action> m_JoystickBinding;
	std::map<unsigned int, Action> m_JoystickButtonBinding;
	
	bool m_IsUsingJoystick;
	unsigned int m_JoystickIndex;
};