#pragma once

#include "Threat.h"

namespace SeekASoul
{
	namespace AI
	{
		class ThreatManager 
		{
		public:
			static ThreatManager* GetInstance();
			ThreatManager(const ThreatManager& threatManager) = delete;
			void operator=(const ThreatManager& threatManager) = delete;

			void Update();

			void RegisterThreat(Threat* threat, sf::Vector2f* threatPosition);
			void UnregisterThreat(Threat* threat);

		private:
			ThreatManager();
			~ThreatManager();

			void UpdateThreats();

			//====================//

			static ThreatManager* m_ThreatManager; // Singleton instance

			std::map<Threat*, sf::Vector2f*> m_Threats;
		};
	}
}