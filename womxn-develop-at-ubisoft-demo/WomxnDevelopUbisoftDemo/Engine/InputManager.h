#pragma once

#include <memory>

class InputManager 
{
public:
	InputManager();

	InputManager(const InputManager& inputManager);

	void UpdateMousePosition(const sf::RenderWindow& gameWindow);

	inline const sf::Vector2f GetMousePosition() 
	{
		return m_MousePosition;
	}

	/*inline const sf::Keyboard::Key GetKey(std::string actionName)
	{
		return m_keyboardBind.at(actionName);
	}

	inline const sf::Joystick::Axis GetAxis(std::string actionName)
	{
		return m_controllerBind.at(actionName);
	}*/

private:
	
	sf::Vector2f m_MousePosition;
	
	//std::map<std::string, sf::Keyboard::Key> m_keyboardBind;
	//std::map<std::string, sf::Joystick::Axis> m_controllerBind;
};