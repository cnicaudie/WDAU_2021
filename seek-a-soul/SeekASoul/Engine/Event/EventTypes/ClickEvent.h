#pragma once

#include "Event.h"

namespace SeekASoul
{
	namespace Engine
	{
		class ClickEvent : public Event 
		{
		public:
			ClickEvent(const sf::Vector2f& clickPosition) : Event(EventType::CLICK), m_ClickPosition(clickPosition) {};

			virtual bool operator==(const Event& other) const override
			{
				if (const ClickEvent* otherClickEvent = dynamic_cast<const ClickEvent*>(&other))
				{
					return m_ClickPosition == otherClickEvent->m_ClickPosition;
				}

				return false;
			};

			inline const sf::Vector2f GetClickPosition() const { return m_ClickPosition; };
	
		private:
			sf::Vector2f m_ClickPosition;
		};
	}
}