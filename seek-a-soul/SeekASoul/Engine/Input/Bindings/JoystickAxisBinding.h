#pragma once

#include "Binding.h"

class JoystickAxisBinding : public Binding
{
public:
	JoystickAxisBinding(const sf::Joystick::Axis& axis, const bool isPositive, const float axisPosition = 0.f);

	bool operator==(Binding* other) override;

	inline const float GetAxisPosition() const { return m_AxisPosition; };

private:
	sf::Joystick::Axis m_Axis;
	bool m_IsPositive;
	float m_AxisPosition;
};