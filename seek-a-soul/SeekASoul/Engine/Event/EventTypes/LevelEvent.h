#pragma once

#include "Event.h"

namespace SeekASoul
{
	namespace Engine
	{
		class LevelEvent : public Event
		{
		public:
			LevelEvent(SeekASoul::Gameplay::LevelStatus levelStatus) : Event(EventType::LEVEL), m_LevelStatus(levelStatus) {};

			virtual bool operator==(const Event& other) const override
			{
				if (const LevelEvent* otherLevelEvent = dynamic_cast<const LevelEvent*>(&other))
				{
					return m_LevelStatus == otherLevelEvent->m_LevelStatus;
				}

				return false;
			};

			inline const SeekASoul::Gameplay::LevelStatus GetLevelStatus() const { return m_LevelStatus; };
	
		private:
			SeekASoul::Gameplay::LevelStatus m_LevelStatus;
		};
	}
}