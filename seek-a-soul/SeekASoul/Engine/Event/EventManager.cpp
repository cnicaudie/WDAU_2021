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
	for (auto pair : m_EventListeners) 
	{
		for (IEventHandler* handler : pair.second) 
		{
			delete handler;
		}
	}
}

void EventManager::Update(float deltaTime)
{
	for (EventType e : m_EventsToFire)
	{
		for (IEventHandler* handler : m_EventListeners[e])
		{
			handler->Fire();
		}
	}

	m_EventsToFire.clear();
}

void EventManager::AddListener(const Event& eventType, IEventHandler* newEventHandler)
{
	std::vector<IEventHandler*>& handlerList = m_EventListeners[eventType.GetType()];

	if (std::find(handlerList.begin(), handlerList.end(), newEventHandler) != handlerList.end())
	{
		return;
	}

	handlerList.push_back(newEventHandler);
}

void EventManager::RemoveListener(const Event& eventType, IEventHandler* newEventHandler)
{
	std::vector<IEventHandler*>& handlerList = m_EventListeners[eventType.GetType()];

	auto pos = std::find(handlerList.begin(), handlerList.end(), newEventHandler);
	if (pos == handlerList.end())
	{
		return;
	}

	handlerList.erase(pos);
	delete (*pos);
}

void EventManager::Fire(const Event& eventType)
{
	EventType e = eventType.GetType();
	
	if (std::find(m_EventsToFire.begin(), m_EventsToFire.end(), e) == m_EventsToFire.end())
	{
		m_EventsToFire.push_back(e);
	}
}