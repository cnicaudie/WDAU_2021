#pragma once

#include <AI/AIEntity.h>

namespace SeekASoul 
{
	namespace AI 
	{
		class AIStrategy 
		{
		public:
			virtual void ExecuteStrategy(AIEntity* ai) const = 0;
		};
	}
}