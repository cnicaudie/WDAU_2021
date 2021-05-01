#include <stdafx.h>
#include "AIEntity.h"
#include "AIManager.h"
#include <AI/Strategies/AIStrategy.h>

namespace SeekASoul
{
	namespace AI
	{
		AIEntity::AIEntity(const ThreatTeam& teamName)
			: AIAction()
			, Threat(ThreatLevel::CALM, teamName)
		{
			static int counter = 0;
			m_ID = counter++;

			AIManager::GetInstance()->Register(this);
		}

		AIEntity::~AIEntity() 
		{
			AIManager::GetInstance()->Unregister(this);
		}
	}
}