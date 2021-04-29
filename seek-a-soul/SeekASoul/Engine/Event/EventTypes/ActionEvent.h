#pragma once

#include "Event.h"
#include <Engine/Input/IAction.h>

namespace SeekASoul
{
	namespace Engine
	{
		class ActionEvent : public Event 
		{
		public:
			ActionEvent(IAction* action, const float actionScale = 1.f, const sf::Vector2f& actionDirection = { 0.f, 0.f }
				, const bool isPointDirection = true, const bool isAim = false)
				: Event(EventType::ACTION)
				, m_Action(action)
				, m_ActionScale(actionScale)
				, m_ActionDirection(actionDirection)
				, m_IsPointDirection(isPointDirection)
				, m_IsAim(isAim)
			{};

			virtual bool operator==(const Event& other) const override
			{
				if (const ActionEvent* otherActionEvent = dynamic_cast<const ActionEvent*>(&other)) 
				{
					return m_Action == otherActionEvent->m_Action;
				}

				return false;
			};

			inline IAction* GetAction() const { return m_Action; };
			inline const float GetActionScale() const { return m_ActionScale; };
			inline const sf::Vector2f GetActionDirection() const { return m_ActionDirection; };
			inline const bool IsPointActionDirection() const { return m_IsPointDirection; };
			inline const bool IsAim() const { return m_IsAim; };

		private:
			IAction* m_Action;
			float m_ActionScale;
			sf::Vector2f m_ActionDirection;
			bool m_IsPointDirection;
			bool m_IsAim;
		};
	}
}