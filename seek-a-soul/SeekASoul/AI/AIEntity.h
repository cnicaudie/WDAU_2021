#pragma once

#include "AIAction.h"
#include <AI/Threat/Threat.h>

namespace SeekASoul
{
	namespace AI
	{
		class AIStrategy;

		class AIEntity : public AIAction, public Threat
		{
		public:
			AIEntity(std::unique_ptr<AIStrategy> strategy);
			
			virtual const bool operator==(const AIEntity& other) 
			{
				m_ID == other.m_ID;
			}

		private:
			int m_ID;
		};
	}
}