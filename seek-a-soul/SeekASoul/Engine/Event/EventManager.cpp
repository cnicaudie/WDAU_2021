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

void EventManager::Update() 
{
	for (std::shared_ptr<Event> evnt : m_EventsToFire) 
	{
		auto it = m_EventListeners.find(typeid(*evnt.get()));

		if (it != m_EventListeners.end())
		{
			for (std::unique_ptr<IEventListener>& listener : it->second)
			{
				listener->Fire(evnt.get());
			}
		}
	}

	m_EventsToFire.clear();
}

void EventManager::Fire(const std::shared_ptr<Event>& evnt)
{	
	auto it = std::find(m_EventsToFire.begin(), m_EventsToFire.end(), evnt);

	if (it != m_EventsToFire.end()) { return; }

	m_EventsToFire.push_back(evnt);
}