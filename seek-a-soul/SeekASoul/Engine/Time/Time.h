#pragma once

#include <cstdint>
#include <chrono>

namespace Time 
{
	static const int64_t GetCurrentTimeAsMilliseconds() 
	{
		auto currentTime = std::chrono::system_clock::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime).count();
	}
}