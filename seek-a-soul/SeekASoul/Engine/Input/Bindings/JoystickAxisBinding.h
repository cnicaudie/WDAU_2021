#pragma once

#include "Binding.h"

class JoystickAxisBinding : public Binding
{
public:
	JoystickAxisBinding(const sf::Joystick::Axis& axis, const bool isPositive);

	bool operator==(Binding* other) override;

private:
	sf::Joystick::Axis m_Axis;
	bool m_IsPositive;
};