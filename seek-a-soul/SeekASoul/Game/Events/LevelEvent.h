#pragma once

#include <Engine/Event/EventTypes/Event.h>
#include <Game/Level/LevelStatus.h>

namespace SeekASoul
{
	namespace Gameplay
	{
		class LevelEvent : public Engine::Event
		{
		public:
			LevelEvent(LevelStatus levelStatus) : Engine::Event(Engine::EventType::LEVEL), m_LevelStatus(levelStatus) {};

			virtual bool operator==(const Event& other) const override
			{
				if (const LevelEvent* otherLevelEvent = dynamic_cast<const LevelEvent*>(&other))
				{
					return m_LevelStatus == otherLevelEvent->m_LevelStatus;
				}

				return false;
			};

			inline const LevelStatus GetLevelStatus() const { return m_LevelStatus; };
	
		private:
			LevelStatus m_LevelStatus;
		};
	}
}