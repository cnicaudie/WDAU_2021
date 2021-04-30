#include <stdafx.h>
#include "AIManager.h"
#include "AIEntity.h"
#include <AI/Strategies/AIPatrol.h>

namespace SeekASoul
{
	namespace AI
	{
		AIManager* AIManager::m_AIManager = nullptr;

		AIManager* AIManager::GetInstance()
		{
			if (m_AIManager == nullptr)
			{
				m_AIManager = new AIManager();
			}

			return m_AIManager;
		}

		AIManager::AIManager()
		{
			// TODO
		}

		AIManager::~AIManager()
		{
			delete m_AIManager;
		}

		void AIManager::Update() 
		{
			UpdateStrategies();
		}

		void AIManager::UpdateStrategies()
		{
			for (AIEntity* ai : m_AIs) 
			{
				switch (ai->GetThreatLevel()) 
				{
					case ThreatLevel::CALM:
					{
						// TODO
						break;
					}

					case ThreatLevel::ALERT:
					{
						AIPatrol test;
						test.ExecuteStrategy(ai);
						break;
					}

					case ThreatLevel::IN_DANGER:
					{
						// TODO : Follow/attack strategy
						break;
					}

					default:
						break;
				}
			}
		}

		void AIManager::Register(AIEntity* ai)
		{
			m_AIs.push_back(ai);
		}
		
		void AIManager::Unregister(AIEntity* ai)
		{
			auto const& pos = std::find_if(m_AIs.begin(), m_AIs.end(), [&](AIEntity* other) { return *ai == other; });

			if (pos != m_AIs.end())
			{
				m_AIs.erase(pos);
			}
		}
	}
}