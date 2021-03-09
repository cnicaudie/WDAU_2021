#pragma once
#include <Engine/Event/EventType.h>

class Event
{
public:
	Event();

	inline void Fire()
	{
		EventManager::GetInstance()->Fire(*this);
	};

	inline const EventType GetType() const { return m_EventType; };

private:
	EventType m_EventType;
};