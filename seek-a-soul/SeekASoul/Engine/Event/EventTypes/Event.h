#pragma once

#include "EventType.h"

class Event
{
public:
	Event(EventType eventType, Action action = Action::NONE) : m_EventType(eventType), m_Action(action) {};
	
	// Necessary to be a key in EventManager's std::map
	/*virtual bool operator<(const Event& other) const
	{
		if (m_EventType == other.m_EventType && m_EventType == EventType::ACTION) 
		{
			return m_Action < other.m_Action;
		}

		return m_EventType < other.m_EventType;
	}*/

	virtual bool operator==(const Event& other) const
	{
		return m_EventType == other.m_EventType;
	};

	inline const EventType GetEventType() const { return m_EventType; };

private:
	EventType m_EventType;
	Action m_Action;
};