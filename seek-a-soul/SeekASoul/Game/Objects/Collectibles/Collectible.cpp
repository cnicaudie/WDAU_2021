#include <stdafx.h>
#include "Collectible.h"
#include <Game/Entities/Player.h>

namespace SeekASoul
{
	namespace Gameplay
	{
		Collectible::Collectible() 
			: m_WasCollected(false)
		{}

		Collectible::~Collectible()
		{
			LOG_INFO("Collectible destroyed !");
		}

		void Collectible::OnTrigger(Engine::BoxCollideable* other)
		{
			if (typeid(*other) == typeid(class Player) && !m_WasCollected)
			{
				LOG_INFO("Player collected something !");
				m_WasCollected = true;
			}
		}
	}
}