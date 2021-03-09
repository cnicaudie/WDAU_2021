#pragma once

#include "Binding.h"

class JoystickButtonBinding : public Binding
{
public:
	JoystickButtonBinding(const unsigned int& button);

	int GetBinding() override;

private:
	unsigned int m_Button;
};