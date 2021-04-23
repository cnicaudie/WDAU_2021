#pragma once

#include <Engine/Event/Listener/EventListener.h>
#include <typeindex>

class EventManager 
{
public:
	static EventManager* GetInstance();
	EventManager(const EventManager& gameManager) = delete;
	void operator=(const EventManager& gameManager) = delete;

	void Update();

	template <typename T, typename EventT>
	void AddListener(const EventListener<T, EventT>& eventListener)
	{
		std::vector<std::unique_ptr<IEventListener>>& listenerList = m_EventListeners[typeid(EventT)];
		listenerList.push_back(std::make_unique<EventListener<T, EventT>>(eventListener));
	};

	template <typename T, typename EventT>
	void RemoveListener(const EventListener<T, EventT>& eventListener)
	{
		std::vector<std::unique_ptr<IEventListener>>& listenerList = m_EventListeners[typeid(EventT)];
		
		auto const& pos = std::find_if(listenerList.begin(), listenerList.end(), [&](std::unique_ptr<IEventListener>& ptr) 
			{ 
				return eventListener == ptr.get(); 
			});

		if (pos == listenerList.end()) { return; }

		listenerList.erase(pos);
	};
	
	void Fire(const std::shared_ptr<Event>& evnt);

private:
	EventManager();
	~EventManager();

	//====================//

	static EventManager* m_EventManager; // Singleton instance

	std::map<std::type_index, std::vector<std::unique_ptr<IEventListener>>> m_EventListeners;
	std::vector<std::shared_ptr<Event>> m_EventsToFire;
};