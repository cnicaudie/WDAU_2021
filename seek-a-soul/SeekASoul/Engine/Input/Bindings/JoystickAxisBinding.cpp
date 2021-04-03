#include <stdafx.h>
#include "JoystickAxisBinding.h"

JoystickAxisBinding::JoystickAxisBinding(const sf::Joystick::Axis& axis, const bool isPositive, const float axisPosition) 
	: m_Axis(axis), m_IsPositive(isPositive), m_AxisPosition(axisPosition)
{}

bool JoystickAxisBinding::operator==(Binding* other) 
{
	JoystickAxisBinding* otherJoystickAxis = dynamic_cast<JoystickAxisBinding*>(other);

	return (otherJoystickAxis != nullptr) && (this->m_Axis == otherJoystickAxis->m_Axis) && (this->m_IsPositive == otherJoystickAxis->m_IsPositive);
}