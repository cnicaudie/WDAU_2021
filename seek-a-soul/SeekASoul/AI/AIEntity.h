#pragma once

#include "AIAction.h"
#include <AI/Threat/Threat.h>
#include <AI/Strategies/AIStrategyType.h>

namespace SeekASoul
{
	namespace AI
	{
		class AIStrategy;

		class AIEntity : public AIAction, public Threat
		{
		public:
			AIEntity(const ThreatTeam& teamName);
			~AIEntity();

			void SetStrategy(AIStrategyType strategyType) { m_Strategy = strategyType; };
			
			virtual const bool operator==(Threat* other) const override
			{
				AIEntity* otherAIEntity = dynamic_cast<AIEntity*>(other);

				if (otherAIEntity != nullptr)
				{
					return m_ID == otherAIEntity->m_ID;
				}
				else
				{
					return false;
				}
			}

		protected:
			AIStrategyType m_Strategy;

		private:
			int m_ID;
		};
	}
}