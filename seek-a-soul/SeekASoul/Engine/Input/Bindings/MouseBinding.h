#pragma once

#include "Binding.h"

class MouseBinding : public Binding
{
public:
	MouseBinding(const sf::Mouse::Button& button);

	bool operator==(Binding* other) override;

private:
	sf::Mouse::Button m_Button;
};