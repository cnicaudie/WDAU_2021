#pragma once

#include <cstdint>

enum class CollisionDirection : std::int32_t
{
	NONE		= 0b00000000,
	BOTTOM		= 0b00000001,
	TOP			= 0b00000010,
	IN_BOTTOM	= 0b00000100,
	IN_TOP		= 0b00001000,
	LEFT		= 0b00010000,
	RIGHT		= 0b00100000,
	IN_LEFT		= 0b01000000,
	IN_RIGHT	= 0b10000000,
	ALL			= 0b11111111,
};