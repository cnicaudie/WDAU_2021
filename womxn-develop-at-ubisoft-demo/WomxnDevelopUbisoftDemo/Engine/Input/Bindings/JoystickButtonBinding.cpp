#include <stdafx.h>
#include "JoystickButtonBinding.h"

JoystickButtonBinding::JoystickButtonBinding(const unsigned int& button) : m_Button(button) {}

int JoystickButtonBinding::GetBinding() { return m_Button; }