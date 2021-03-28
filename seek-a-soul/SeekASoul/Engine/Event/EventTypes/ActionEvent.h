#pragma once

#include "Event.h"

class ActionEvent : public Event 
{
public:
	ActionEvent(Action action, float joystickAxisPosition, sf::Vector2f mousePosition = { 0.f, 0.f })
		: Event(EventType::ACTION)
		, m_Action(action)
		, m_JoystickAxisPosition(joystickAxisPosition)
		, m_MousePosition(mousePosition)
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
	inline const float GetJoystickAxisPosition() const { return m_JoystickAxisPosition; };
	//inline const sf::Vector2f GetShootDirection() const { return m_ShootDirection; };

private:
	Action m_Action;
	float m_JoystickAxisPosition;
	sf::Vector2f m_MousePosition;
};