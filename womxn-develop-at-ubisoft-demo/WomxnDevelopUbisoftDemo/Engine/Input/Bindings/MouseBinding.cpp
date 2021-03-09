#include <stdafx.h>
#include "MouseBinding.h"

MouseBinding::MouseBinding(const sf::Mouse::Button& button) : m_Button(button) {}

int MouseBinding::GetBinding() { return m_Button; }