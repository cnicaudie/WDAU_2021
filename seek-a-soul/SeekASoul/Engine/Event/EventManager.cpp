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
	for (Event& e : m_EventsToFire)
	{
		for (std::unique_ptr<IEventListener>& listener : m_EventListeners[typeid(e)])
		{
			listener->Fire(&e);
		}
	}

	m_EventsToFire.clear();
}

void EventManager::Fire(const Event& evnt)
{	
	auto it = std::find(m_EventsToFire.begin(), m_EventsToFire.end(), evnt);
	
	if (it != m_EventsToFire.end()) { return; }

	m_EventsToFire.push_back(evnt);

	/*auto it = m_EventListeners.find(typeid(*evnt));
	if (it != m_EventListeners.end())
	{
		for (std::unique_ptr<IEventListener>& listener : it->second)
		{
			listener->Fire(evnt);
		}
	}*/
}