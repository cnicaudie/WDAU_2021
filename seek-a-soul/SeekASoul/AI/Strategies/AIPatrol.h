#pragma once

#include "AIStrategy.h"

namespace SeekASoul
{
	namespace AI
	{
		class AIPatrol : public AIStrategy 
		{
		public:
			AIPatrol();

			virtual void ExecuteStrategy(AIEntity* ai) const override;

			virtual AIStrategyType GetStrategyType() const override { return AIStrategyType::PATROL; };
		};
	}
}