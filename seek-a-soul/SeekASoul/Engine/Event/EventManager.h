#pragma once
#include "Event.h"
#include "IEventHandler.h"

class EventManager 
{
public:
	static EventManager* GetInstance();
	EventManager(const EventManager& gameManager) = delete;
	void operator=(const EventManager& gameManager) = delete;

	void Update(float deltaTime);

	void AddListener(const Event& eventType, IEventHandler* newEventHandler);
	void RemoveListener(const Event& eventType, IEventHandler* newEventHandler);
	
	void Fire(const Event& eventType);

private:
	EventManager();
	~EventManager();

	//====================//

	static EventManager* m_EventManager; // Singleton instance

	std::map<EventType, std::vector<IEventHandler*>> m_EventListeners;
	std::vector<EventType> m_EventsToFire;
};