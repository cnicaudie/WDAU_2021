#include <stdafx.h>
#include "ThreatManager.h"

namespace SeekASoul
{
	namespace AI
	{
		ThreatManager* ThreatManager::m_ThreatManager = nullptr;

		ThreatManager* ThreatManager::GetInstance()
		{
			if (m_ThreatManager == nullptr)
			{
				m_ThreatManager = new ThreatManager();
			}

			return m_ThreatManager;
		}

		ThreatManager::ThreatManager()
		{
			// TODO
		}

		ThreatManager::~ThreatManager()
		{
			delete m_ThreatManager;
		}

		void ThreatManager::RegisterThreat(Threat* threat, sf::Vector2f* threatPosition) 
		{
			m_Threats.emplace(threat, threatPosition);
		}

		void ThreatManager::UnregisterThreat(Threat* threat) 
		{
			auto const& pos = std::find_if(m_Threats.begin(), m_Threats.end(), [&](std::pair<Threat*, sf::Vector2f*> other)
				{ 
					return *threat == other.first; 
				});

			if (pos != m_Threats.end())
			{
				m_Threats.erase(pos);
			}
		}
	}
}