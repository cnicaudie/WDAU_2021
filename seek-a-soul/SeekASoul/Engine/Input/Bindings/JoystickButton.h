#pragma once

namespace SeekASoul 
{
	namespace Engine 
	{
		enum class JoystickButton : unsigned int
		{
			// FORMAT : HANDSIDE_BUTTONSIDE
			RIGHT_BOTTOM	= 0, // X on PS3 Dualshock
			RIGHT_RIGHT		= 1, // O on PS3 Dualshock
			RIGHT_LEFT		= 2, // Square on PS3 Dualshock
			RIGHT_TOP		= 3, // Triangle on PS3 Dualshock
			TOP_LEFT		= 4, // L1 on PS3 Dualshock
			TOP_RIGHT		= 5, // R1 on PS3 Dualshock
			MIDDLE_LEFT		= 6, // Select on PS3 Dualshock
			MIDDLE_RIGHT	= 7, // Start on PS3 Dualshock
			JOYSTICK_LEFT	= 8, // Left joystick on PS3 Dualshock
			JOYSTICK_RIGHT	= 9, // Right joystick on PS3 Dualshock
			MAX_VALUE		= 10 // Specific entry to check if pressed button is supported
		};
	}
}