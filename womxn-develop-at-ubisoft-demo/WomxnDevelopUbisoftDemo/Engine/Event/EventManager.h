#pragma once
#include <Engine/Event/Event.h>

class EventManager 
{
public:
	static EventManager* GetInstance();
	EventManager(const EventManager& gameManager) = delete;
	void operator=(const EventManager& gameManager) = delete;

	void Update(float deltaTime);
	
	template<typename T>
	void AddListener(const std::function<void(const T& listener, const Event&)>& function);
	//void RemoveListener(const std::function<void(const Event&)>& function); 
	void Fire(const Event& event);

private:
	EventManager();
	~EventManager();

	static EventManager* m_EventManager; // Singleton instance

	std::map<EventType, std::vector<const std::function<void(const Event&)>>> m_EventListeners;
};