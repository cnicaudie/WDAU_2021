#pragma once

#include "Binding.h"

namespace SeekASoul
{
	namespace Engine
	{
		class JoystickButtonBinding : public Binding
		{
		public:
			JoystickButtonBinding(const unsigned int& button);

			bool operator==(Binding* other) override;

		private:
			unsigned int m_Button;
		};
	}
}