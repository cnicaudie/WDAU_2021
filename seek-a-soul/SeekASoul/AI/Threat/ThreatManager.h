#pragma once

#include "Threat.h"

namespace SeekASoul
{
	namespace AI
	{
		class ThreatManager 
		{
		public:
			ThreatManager();

			void RegisterThreat(const Threat& threat);
			void UnregisterThreat(const Threat& threat);

		private:
			std::vector<Threat> m_Threats;
		};
	}
}