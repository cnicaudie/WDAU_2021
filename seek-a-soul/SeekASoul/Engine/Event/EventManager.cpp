#include <stdafx.h>
#include "EventManager.h"

EventManager* EventManager::m_EventManager = nullptr;

EventManager* EventManager::GetInstance()
{
    if (m_EventManager == nullptr)
    {
        m_EventManager = new EventManager();
    }

    return m_EventManager;
}

EventManager::EventManager() {}

EventManager::~EventManager() 
{
	delete m_EventManager;
}

void EventManager::Update(float deltaTime)
{
	for (const Event& e : m_EventsToFire)
	{
		for (std::unique_ptr<IEventListener>& listener : m_EventListeners[e])
		{
			listener->Fire();
		}
	}

	m_EventsToFire.clear();
}

void EventManager::Fire(const Event& eventType)
{	
	m_EventsToFire.insert(eventType);
}