#include <stdafx.h>
#include "KeyboardBinding.h"

KeyboardBinding::KeyboardBinding(const sf::Keyboard::Key& key) : m_Key(key) {}

bool KeyboardBinding::operator==(Binding* other)
{
	KeyboardBinding* otherKey = dynamic_cast<KeyboardBinding*>(other);
	return (otherKey != nullptr) && (this->m_Key == otherKey->m_Key);
}