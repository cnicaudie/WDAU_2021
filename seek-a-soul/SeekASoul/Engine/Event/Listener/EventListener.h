#pragma once

#include "IEventListener.h"
#include <functional>

class Event;

template<typename T, typename EventT>
class EventListener : public IEventListener
{
public:
	EventListener(T* instance, std::function<void(T*, EventT*)> callbackFunction) : m_Instance(instance), m_CallbackFunction(callbackFunction) {};
	~EventListener() {};

	bool operator==(IEventListener* other) const override
	{
		EventListener* otherListener = dynamic_cast<EventListener*>(other);
		
		if (otherListener != nullptr) 
		{
			return typeid(m_Instance).name() == typeid(otherListener->m_Instance).name() 
				&& m_CallbackFunction.target_type() == otherListener->m_CallbackFunction.target_type();			
		} 
		else 
		{
			return false;
		}
	}

	void Fire(Event* evnt) const override
	{
		EventT* evntType = static_cast<EventT*>(evnt); // Safety cast
		m_CallbackFunction(m_Instance, evntType);
	};

private:
	T* m_Instance;
	std::function<void(T*, EventT*)> m_CallbackFunction;
};