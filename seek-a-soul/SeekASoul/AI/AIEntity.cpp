#include <stdafx.h>
#include "AIEntity.h"
#include "AIManager.h"

namespace SeekASoul
{
	namespace AI
	{
		AIEntity::AIEntity(std::unique_ptr<AIStrategy> strategy)
			: AIAction()
			, Threat(ThreatLevel::CALM)
		{
			static int counter = 0;
			m_ID = counter++;

			AIManager::GetInstance()->Register(this);
		}
	}
}