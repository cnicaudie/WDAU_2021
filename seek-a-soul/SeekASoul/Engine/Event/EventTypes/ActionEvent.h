#pragma once

#include "Event.h"

class ActionEvent : public Event 
{
public:
	ActionEvent(Action action, float joystickAxisPosition)
		: Event(EventType::ACTION)
		, m_Action(action)
		, m_ActionScale(joystickAxisPosition)
	{};

	virtual bool operator==(const Event& other) const override
	{
		if (const ActionEvent* otherActionEvent = dynamic_cast<const ActionEvent*>(&other)) 
		{
			return m_Action == otherActionEvent->m_Action;
		}

		return false;
	};

	inline const Action GetActionType() const { return m_Action; };
	inline const float GetActionScale() const { return m_ActionScale; };

private:
	Action m_Action;
	float m_ActionScale;
};