#include <stdafx.h>
#include "JoystickAxisBinding.h"

JoystickAxisBinding::JoystickAxisBinding(const sf::Joystick::Axis& axis, const bool isPositive) : m_Axis(axis), m_IsPositive(isPositive) {}

bool JoystickAxisBinding::operator==(Binding* other) 
{
	JoystickAxisBinding* otherJoystickAxis = dynamic_cast<JoystickAxisBinding*>(other);

	return (otherJoystickAxis != nullptr) && (this->m_Axis == otherJoystickAxis->m_Axis) && (this->m_IsPositive == otherJoystickAxis->m_IsPositive);
}