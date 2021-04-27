#include <stdafx.h>
#include "JoystickButtonBinding.h"

namespace SeekASoul
{
	namespace Engine
	{
		JoystickButtonBinding::JoystickButtonBinding(const unsigned int& button) : m_Button(button) {}

		bool JoystickButtonBinding::operator==(Binding* other)
		{
			JoystickButtonBinding* otherJoystickButton = dynamic_cast<JoystickButtonBinding*>(other);
			return (otherJoystickButton != nullptr) && (this->m_Button == otherJoystickButton->m_Button);
		}
	}
}