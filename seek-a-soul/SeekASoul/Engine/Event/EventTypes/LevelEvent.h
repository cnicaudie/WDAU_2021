#pragma once

#include "Event.h"

class LevelEvent : public Event
{
public:
	LevelEvent(EndLevelType endLevelType) : Event(EventType::END_LEVEL), m_EndLevelType(endLevelType) {};

	virtual bool operator==(const Event& other) const override
	{
		if (const LevelEvent* otherLevelEvent = dynamic_cast<const LevelEvent*>(&other))
		{
			return m_EndLevelType == otherLevelEvent->m_EndLevelType;
		}

		return false;
	};

	inline const EndLevelType GetEndLevelType() const { return m_EndLevelType; };
	
private:
	EndLevelType m_EndLevelType;
};