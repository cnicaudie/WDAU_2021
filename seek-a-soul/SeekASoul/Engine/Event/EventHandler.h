#pragma once
#include "IEventHandler.h"
#include "Event.h"
#include <iostream>
#include <functional>

template<typename T>
class EventHandler : public IEventHandler
{
public:
	EventHandler(T* instance, std::function<void(T*)> callbackFunction) : m_Instance(instance), m_CallbackFunction(callbackFunction) {};

	bool operator==(IEventHandler* other) const override 
	{
		EventHandler* otherHandler = dynamic_cast<EventHandler*>(other);
		return m_Instance == otherHandler->m_Instance && m_CallbackFunction.target_type() == otherHandler->m_CallbackFunction.target_type();
	}

	void Fire() const override
	{
		m_CallbackFunction(m_Instance);
	};

private:
	T* m_Instance;
	std::function<void(T*)> m_CallbackFunction;
};