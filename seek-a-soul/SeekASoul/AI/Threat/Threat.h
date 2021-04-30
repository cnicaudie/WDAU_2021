#pragma once

#include "ThreatLevel.h"
#include "ThreatTeam.h"

namespace SeekASoul
{
	namespace AI
	{
		class Threat 
		{
		public:
			Threat(ThreatLevel threatLevel, ThreatTeam teamName);
			~Threat();

			virtual const bool operator==(Threat* other) const = 0;

			inline void SetThreatLevel(ThreatLevel threatLevel) { m_ThreatLevel = threatLevel; };
			inline const ThreatLevel GetThreatLevel() const { return m_ThreatLevel; };
			inline const ThreatTeam GetTeamName() const { return m_TeamName; };

		private:
			ThreatLevel m_ThreatLevel;
			ThreatTeam m_TeamName;
		};
	}
}