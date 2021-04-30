#pragma once

#include "Binding.h"
#include <Engine/Input/Bindings/JoystickButton.h>

namespace SeekASoul
{
	namespace Engine
	{
		class JoystickButtonBinding : public Binding
		{
		public:
			JoystickButtonBinding(const JoystickButton button);

			bool operator==(Binding* other) override;

		private:
			JoystickButton m_Button;
		};
	}
}