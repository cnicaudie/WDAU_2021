#include <stdafx.h>
#include "MouseBinding.h"

MouseBinding::MouseBinding(const sf::Mouse::Button& button) : m_Button(button) {}

bool MouseBinding::operator==(Binding* other)
{
	MouseBinding* otherMouseButton = dynamic_cast<MouseBinding*>(other);
	return (otherMouseButton != nullptr) && (this->m_Button == otherMouseButton->m_Button);
}