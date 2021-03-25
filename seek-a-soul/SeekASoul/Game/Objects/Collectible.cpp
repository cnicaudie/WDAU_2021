#include <stdafx.h>
#include "Collectible.h"

Collectible::Collectible() 
	: m_WasCollected(false)
{}

void Collectible::OnTrigger(BoxCollideable* other)
{
	if (typeid(*other) == typeid(class Player) && !m_WasCollected)
	{
		std::cout << "Collected soul chunk !" << std::endl;
		m_WasCollected = true;
	}
}