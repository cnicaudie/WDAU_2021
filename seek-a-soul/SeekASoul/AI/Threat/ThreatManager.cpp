#include <stdafx.h>
#include "ThreatManager.h"
#include <Engine/Maths/Maths.h>

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

		void ThreatManager::Update()
		{
			UpdateThreats();
		}

		void ThreatManager::UpdateThreats()
		{
			for (auto pair : m_Threats) 
			{
				pair.first->ResetThreatLevel();
			}

			for (auto it1 = m_Threats.begin(); it1 != m_Threats.end(); it1++) 
			{
				for (auto it2 = m_Threats.begin(); it2 != it1 && it2 != m_Threats.end(); it2++)
				{
					Threat* threat1 = it1->first;
					Threat* threat2 = it2->first;

					if (threat1->GetTeamName() != threat2->GetTeamName())
					{
						const float ALERT_DISTANCE_THRESHOLD = 500.f;
						const float FOLLOW_DISTANCE_THRESHOLD = 200.f;

						const float distance = Engine::Maths::GetDistance(*(it1->second), *(it2->second));

						if (distance <= ALERT_DISTANCE_THRESHOLD)
						{
							threat1->SetThreatLevel(ThreatLevel::ALERT);
							threat2->SetThreatLevel(ThreatLevel::ALERT);
						}
					}
				}
			}
		}

		void ThreatManager::RegisterThreat(Threat* threat, sf::Vector2f* threatPosition) 
		{
			m_Threats.emplace(threat, threatPosition);
			LOG_INFO("Registered threat : " << typeid(*threat).name());
		}

		void ThreatManager::UnregisterThreat(Threat* threat) 
		{
			LOG_INFO("Unregistered threat : " << typeid(*threat).name());

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