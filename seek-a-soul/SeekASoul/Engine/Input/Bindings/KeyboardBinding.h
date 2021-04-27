#pragma once

#include "Binding.h"

namespace SeekASoul
{
	namespace Engine
	{
		class KeyboardBinding : public Binding
		{
		public:
			KeyboardBinding(const sf::Keyboard::Key& key);

			bool operator==(Binding* other) override;

		private:
			sf::Keyboard::Key m_Key;
		};
	}
}