#pragma once

#include "AIActionType.h"

namespace SeekASoul
{
	namespace AI
	{
		class AIAction 
		{
		public:
			AIAction();

			virtual const bool CanDoAction(AIActionType action) const;

			virtual void Stop();
			virtual void MoveUp();
			virtual void MoveDown();
			virtual void MoveLeft();
			virtual void MoveRight();

			// void Attack();

		protected:
			AIActionType m_PreviousAction;
			AIActionType m_CurrentAction;
			bool m_CanMoveUp;
			bool m_CanMoveDown;
			bool m_CanMoveLeft;
			bool m_CanMoveRight;
		};
	}
}