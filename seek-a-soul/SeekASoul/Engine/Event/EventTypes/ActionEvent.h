#pragma once

class ActionEvent : public Event 
{
public:
	ActionEvent(Action action) : Event(EventType::ACTION), m_Action(action) {};

	virtual bool operator==(const Event& other) const override
	{
		if (const ActionEvent* otherActionEvent = dynamic_cast<const ActionEvent*>(&other)) 
		{
			return m_Action == otherActionEvent->m_Action;
		}

		return false;
	};

	inline const Action GetActionType() const { return m_Action; };

private:
	Action m_Action;
};