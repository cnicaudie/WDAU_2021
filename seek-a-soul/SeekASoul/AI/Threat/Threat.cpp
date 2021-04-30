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
		{}
	}
}