#pragma once

#include "AIAction.h"
#include <AI/Threat/Threat.h>

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

		private:
			int m_ID;
		};
	}
}