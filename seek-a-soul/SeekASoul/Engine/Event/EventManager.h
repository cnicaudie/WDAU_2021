#pragma once

#include "Event.h"
#include "EventListener.h"

class EventManager 
{
public:
	static EventManager* GetInstance();
	EventManager(const EventManager& gameManager) = delete;
	void operator=(const EventManager& gameManager) = delete;

	void Update(float deltaTime);

	template <typename T>
	void AddListener(const Event& eventType, const EventListener<T>& eventListener) 
	{
		std::vector<std::unique_ptr<IEventListener>>& listenerList = m_EventListeners[eventType.GetType()];

		auto const& pos = std::find_if(listenerList.begin(), listenerList.end(), [&](std::unique_ptr<IEventListener>& ptr) 
			{ 
				return eventListener == ptr.get(); 
			});

		if (pos != listenerList.end()) { return; }

		listenerList.push_back(std::make_unique<EventListener<T>>(eventListener));
	};

	template <typename T>
	void RemoveListener(const Event& eventType, const EventListener<T>& eventListener)
	{
		std::vector<std::unique_ptr<IEventListener>>& listenerList = m_EventListeners[eventType.GetType()];
		
		auto const& pos = std::find_if(listenerList.begin(), listenerList.end(), [&](std::unique_ptr<IEventListener>& ptr) 
			{ 
				return eventListener == ptr.get(); 
			});

		if (pos == listenerList.end()) { return; }

		listenerList.erase(pos);
	};
	
	void Fire(const Event& eventType);

private:
	EventManager();
	~EventManager();

	//====================//

	static EventManager* m_EventManager; // Singleton instance

	std::map<EventType, std::vector<std::unique_ptr<IEventListener>>> m_EventListeners;
	std::vector<EventType> m_EventsToFire;
};