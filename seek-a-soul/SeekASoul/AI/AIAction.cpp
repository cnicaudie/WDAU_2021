#include <stdafx.h>
#include "AIAction.h"

namespace SeekASoul
{
	namespace AI
	{
		AIAction::AIAction() 
			: m_CurrentAction(AIActionType::NONE)
			, m_PreviousAction(AIActionType::NONE)
			, m_CanMoveUp(false)
			, m_CanMoveDown(false)
			, m_CanMoveLeft(false)
			, m_CanMoveRight(false)
		{}

		void AIAction::Stop() 
		{
			m_PreviousAction = m_CurrentAction;
			m_CurrentAction = AIActionType::NONE;
		}

		void AIAction::MoveUp()
		{
			m_PreviousAction = m_CurrentAction; 
			m_CurrentAction = AIActionType::MOVE_UP;
		}

		void AIAction::MoveDown()
		{
			m_PreviousAction = m_CurrentAction;
			m_CurrentAction = AIActionType::MOVE_DOWN;
		}

		void AIAction::MoveLeft()
		{
			m_PreviousAction = m_CurrentAction;
			m_CurrentAction = AIActionType::MOVE_LEFT;
		}

		void AIAction::MoveRight()
		{
			m_PreviousAction = m_CurrentAction;
			m_CurrentAction = AIActionType::MOVE_RIGHT;
		}

		const bool AIAction::CanDoAction(AIActionType actionType) const
		{
			switch (actionType) 
			{
				case AIActionType::MOVE_UP:
				{
					return m_CanMoveUp;
				}

				case AIActionType::MOVE_DOWN:
				{
					return m_CanMoveDown;
				}

				case AIActionType::MOVE_LEFT:
				{
					return m_CanMoveLeft;
				}

				case AIActionType::MOVE_RIGHT:
				{
					return m_CanMoveRight;
				}
			}

			return false;
		}
	}
}