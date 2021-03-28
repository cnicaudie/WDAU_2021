#pragma once

#include "Binding.h"

class KeyboardBinding : public Binding
{
public:
	KeyboardBinding(const sf::Keyboard::Key& key);

	bool operator==(Binding* other) override;

private:
	sf::Keyboard::Key m_Key;
};