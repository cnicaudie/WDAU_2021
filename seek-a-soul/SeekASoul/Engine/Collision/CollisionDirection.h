#pragma once

#include <cstdint>

enum class CollisionDirection : std::int32_t
{
	NONE = 0b00000000,
	BOTTOM = 0b00000001,
	TOP = 0b00000010,
	LEFT = 0b00000100,
	RIGHT = 0b00001000,
	ALL = 0b00001111,
};