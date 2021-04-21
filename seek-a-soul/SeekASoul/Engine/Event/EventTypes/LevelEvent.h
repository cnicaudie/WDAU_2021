#pragma once

#include "Event.h"

class LevelEvent : public Event
{
public:
	LevelEvent(LevelStatus levelStatus) : Event(EventType::LEVEL), m_LevelStatus(levelStatus) {};

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