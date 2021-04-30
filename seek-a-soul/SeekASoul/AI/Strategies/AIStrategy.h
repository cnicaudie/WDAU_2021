#pragma once

#include "AIStrategyType.h"
#include <AI/AIEntity.h>

namespace SeekASoul 
{
	namespace AI 
	{
		class AIStrategy 
		{
		public:
			virtual void ExecuteStrategy(AIEntity* ai) const = 0;
			virtual AIStrategyType GetStrategyType() const { return AIStrategyType::NONE; };
		};
	}
}