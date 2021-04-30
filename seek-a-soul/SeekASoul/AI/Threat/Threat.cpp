#include <stdafx.h>
#include "Threat.h"
#include "ThreatManager.h"

namespace SeekASoul
{
	namespace AI
	{
		Threat::Threat(ThreatLevel threatLevel, ThreatTeam teamName)
			: m_ThreatLevel(threatLevel)
			, m_TeamName(teamName)
		{
			// TODO : Register to ThreatManager in subclass
		}

		Threat::~Threat() 
		{
			// TODO : Unregister to ThreatManager
			ThreatManager::GetInstance()->UnregisterThreat(this);
		}
	}
}