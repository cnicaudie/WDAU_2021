#pragma once

#include "Binding.h"

class KeyboardBinding : public Binding
{
public:
	KeyboardBinding(const sf::Keyboard::Key& key);

	int GetBinding() override;

private:
	sf::Keyboard::Key m_Key;
};