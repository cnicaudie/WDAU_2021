#pragma once

#include <AI/Threat/ThreatLevel.h>

namespace SeekASoul
{
	namespace AI
	{
		class Threat 
		{
		public:
			Threat(ThreatLevel threatLevel); // TODO : Team system ?

			inline const ThreatLevel GetThreatLevel() const { return m_ThreatLevel; };

		private:
			ThreatLevel m_ThreatLevel;
		};
	}
}