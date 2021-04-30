#pragma once

#include "ActionType.h"
#include <Engine/Input/IAction.h>

namespace SeekASoul
{
	namespace Gameplay
	{
		class Action : public Engine::IAction 
		{
		public:
			Action(ActionType actionType) : m_ActionType(actionType) {};

			virtual const bool operator==(IAction* other) const override 
			{
				Action* otherAction = dynamic_cast<Action*>(other);
				
				if (otherAction != nullptr)
				{
					return m_ActionType == otherAction->m_ActionType;
				}
				else
				{
					return false;
				}
			};
			
			virtual const bool IsAimX() const override { return m_ActionType == ActionType::AIM_X; };
			virtual const bool IsAimY() const override { return m_ActionType == ActionType::AIM_Y; };

			const ActionType GetActionType() const { return m_ActionType; };
			
		private:
			ActionType m_ActionType;
		};
	}
}