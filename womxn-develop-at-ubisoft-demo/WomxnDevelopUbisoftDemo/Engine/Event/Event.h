#pragma once
#include <Engine/Event/EventType.h>

class Event
{
public:
	Event(EventType eventType) : m_EventType(eventType) {};

	inline const EventType GetType() const { return m_EventType; };

private:
	EventType m_EventType;
};