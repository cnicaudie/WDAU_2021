#include <stdafx.h>
#include "KeyboardBinding.h"

KeyboardBinding::KeyboardBinding(const sf::Keyboard::Key& key) : m_Key(key) {}

int KeyboardBinding::GetBinding() { return m_Key; }