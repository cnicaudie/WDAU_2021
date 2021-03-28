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

void EventManager::Fire(Event& evnt)
{	
	auto it = m_EventListeners.find(typeid(evnt));
	if (it != m_EventListeners.end())
	{
		for (std::unique_ptr<IEventListener>& listener : it->second)
		{
			listener->Fire(&evnt);
		}
	}
}