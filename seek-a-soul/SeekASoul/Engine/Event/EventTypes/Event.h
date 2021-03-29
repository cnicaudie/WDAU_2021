#pragma once

#include "EventType.h"

class Event
{
public:
	Event(EventType eventType) : m_EventType(eventType) {};
	
	virtual bool operator==(const Event& other) const
	{
		return m_EventType == other.m_EventType;
	};

	inline const EventType GetEventType() const { return m_EventType; };

private:
	EventType m_EventType;
};